#include "telemetry.h"
#include "device.h"

Telemetry::Telemetry(QObject *parent)
    : QObject{parent}
{
    QTimer::singleShot(100, this, SLOT(telemetrySlot()));
}

QJsonObject Telemetry::collectDeviceInformation() {
    Device::SysInfo info = Device::getSingleton()->getSystemInfo();

    QJsonObject data;
    data.insert("DeviceUID", info.uid);
    data.insert("SystemVersion", readFile("/opt/version").trimmed());
    data.insert("Device", Device::getSingleton()->getID());
    if(checkconfig("/external_root/opt/root/rooted")) {
        data.insert("DeviceRooted", "true");
    }
    else {
        data.insert("DeviceRooted", "false");
    }
    if(checkconfig("/external_root/opt/developer/valid-key")) {
        data.insert("DeveloperKeyInstalled", "true");
    }
    else {
        data.insert("DeveloperKeyInstalled", "false");
    }
    data.insert("Message", message);
    return data;
}

bool Telemetry::sendDeviceInformation(QJsonObject data) {
    log("Telemetry data to be sent to server: " + QJsonDocument(data).toJson(QJsonDocument::Compact), className);

    QNetworkRequest request(QUrl("http://23.163.0.39:4317/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager nam;
    QNetworkReply * reply = nam.post(request, QJsonDocument(data).toJson());
    while(!reply->isFinished()) {
        QApplication::processEvents();
    }
    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    QString responseDataQstring = QString(responseData);
    log("Telemetry server's response data was: '" + responseDataQstring + "'", className);
    if(responseDataQstring != "Data received successfully") {
        return false;
    }
    else {
        return true;
    }
}

void Telemetry::telemetrySlot() {
    QJsonObject data = collectDeviceInformation();
    if(sendDeviceInformation(data)) {
        writeFile("/mnt/onboard/.adds/inkbox/.config/24-telemetry/asked", "true");
        writeFile("/mnt/onboard/.adds/inkbox/.config/24-telemetry/enabled", "true");
        showToast("Data sent successfully\nThank you!");
    }
    else {
        showToast("Error while sending data");
    }
}
