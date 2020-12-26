#include "headers/drive.h"


Drive::Drive()
{
    google = new QOAuth2AuthorizationCodeFlow;
    google->setScope("https://www.googleapis.com/auth/drive.file");
    QObject::connect(google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    QString settings;
    QFile file;
    file.setFileName("../client_secret.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(settings.toUtf8());

    const auto object = document.object();
    const auto settingsObject = object["installed"].toObject();
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret(settingsObject["client_secret"].toString());
    const auto redirectUris = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUri(redirectUris[0].toString());
    const auto port = static_cast<quint16>(redirectUri.port());

    google->setAuthorizationUrl(authUri);
    google->setClientIdentifier(clientId);
    google->setAccessTokenUrl(tokenUri);
    google->setClientIdentifierSharedKey(clientSecret);
    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    google->setReplyHandler(replyHandler);

    tokenFile = new QFile("file.txt");
}

Drive::~Drive()
{
    delete google;
    delete manager;
    delete reply;
    delete tokenFile;
}

void Drive::uploadToDrive(const QString &filePath)
{
    this->filePath = filePath;
    connect(google, &QOAuth2AuthorizationCodeFlow::granted, this, &Drive::saveTokenAndConnect);

    google->grant();
}

void Drive::saveTokenAndConnect()
{
    auto access_token = google->token();
    tokenFile->open(QIODevice::WriteOnly | QIODevice::Text);
    tokenFile->write(access_token.toStdString().c_str());
    tokenFile->close();

    manager = new QNetworkAccessManager();
    QNetworkRequest requestGet(QUrl("https://www.googleapis.com/drive/v3/files"));
    QString headerData = "Bearer " + access_token;
    requestGet.setRawHeader("Authorization", headerData.toLocal8Bit());
    reply = manager->get(requestGet);
    connect(reply, &QNetworkReply::finished, this, &Drive::postRequest);
}

void Drive::postRequest()
{
    if(reply->error() == QNetworkReply::NoError){
        qDebug() << reply->readAll();

        tokenFile->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream instream(tokenFile);
        QString token = instream.readLine();
        tokenFile->close();

        QNetworkRequest request(QUrl("https://www.googleapis.com/upload/drive/v3/files?uploadType=resumable"));
        QString headerData = "Bearer " + token;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        auto fileName = filePath.right(filePath.size() - filePath.lastIndexOf('/') - 1);

        QJsonObject body;
        body.insert("name", QJsonValue::fromVariant(fileName));
        QJsonDocument bodyDoc(body);
        QByteArray ba = bodyDoc.toJson();

        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json; charset=UTF-8"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, ba.size());

        reply = manager->post(request, ba);
        connect(reply, &QNetworkReply::finished, this, &Drive::putRequest);
    }
}

void Drive::putRequest()
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.toInt() == 200){
        QString locationHeader("Location");
        QString location = reply->rawHeader(locationHeader.toUtf8());

        QFile *fileDrive = new QFile(filePath);
        fileDrive->open(QIODevice::ReadOnly);

        tokenFile->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream instream(tokenFile);
        QString token = instream.readLine();
        tokenFile->close();

        QUrl url(location);
        QNetworkRequest requestPUT(url);
        QString headerData = "Bearer " + token;
        requestPUT.setRawHeader("Authorization", headerData.toLocal8Bit());
        requestPUT.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/PDF"));
        requestPUT.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(QString::number(fileDrive->size())));

        QNetworkAccessManager* mngr3 = new QNetworkAccessManager();
        QNetworkReply* replyFinish= mngr3->put(requestPUT, fileDrive);
        connect(replyFinish, &QNetworkReply::finished, [=](){
            if(replyFinish->error() == QNetworkReply::NoError){
                qDebug() << "SUCCESS";
            }
            fileDrive->close();
            delete  fileDrive;
            return ;
        });

    }
    else
        qDebug() << "error: " << reply->errorString();

}
