#include "headers/drive.h"

//inicijalizacija objekta za autorizaciju
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
    replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    google->setReplyHandler(replyHandler);

    //pravi se fajl koji cuva korisnikov access token
    tokenFile = new QFile("file.txt");
}

Drive::~Drive()
{
    delete manager;
    delete tokenFile;
    delete replyHandler;
    delete google;
}

//trazi se autorizacija od korisnika
void Drive::uploadToDrive(const QString &filePath)
{
    this->filePath = filePath;
    connect(google, &QOAuth2AuthorizationCodeFlow::granted, this, &Drive::saveTokenAndConnect);
    google->grant();
}

//upisivanje tokena u fajl i slanje inicijalnog GET zahteva
void Drive::saveTokenAndConnect()
{
    auto accessToken = google->token();
    tokenFile->open(QIODevice::WriteOnly | QIODevice::Text);
    tokenFile->write(accessToken.toStdString().c_str());
    tokenFile->close();

    manager = new QNetworkAccessManager();
    QNetworkRequest requestGet(QUrl("https://www.googleapis.com/drive/v3/files"));

    QString headerData = "Bearer " + accessToken;
    requestGet.setRawHeader("Authorization", headerData.toLocal8Bit());

    reply = manager->get(requestGet);
    connect(reply, &QNetworkReply::finished, this, &Drive::postRequest);
}

//salje se POST zahtev za slanje fajla
void Drive::postRequest()
{
    if (reply->error() == QNetworkReply::NoError){
        qDebug() << reply->readAll();

        tokenFile->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream instream(tokenFile);
        QString token = instream.readLine();
        tokenFile->close();

        QNetworkRequest request(QUrl("https://www.googleapis.com/upload/drive/v3/files?uploadType=resumable"));
        QString headerData = "Bearer " + token;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        auto fileName = filePath.right(filePath.size() - filePath.lastIndexOf('/') - 1);

        //postavljanje meta podataka fajla koji se salje
        QJsonObject body;
        body.insert("name", QJsonValue::fromVariant(fileName));
        QJsonDocument bodyDoc(body);
        QByteArray requestBody = bodyDoc.toJson();

        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json; charset=UTF-8"));
        request.setHeader(QNetworkRequest::ContentLengthHeader, requestBody.size());

        reply = manager->post(request, requestBody);
        connect(reply, &QNetworkReply::finished, this, &Drive::putRequest);
    }
}

//salje se PUT zahtev za slanje fajla
void Drive::putRequest()
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    //proverava se status code odgovora POST zahteva
    if (statusCode.toInt() == 200) {
        QString locationHeader("Location");

        //postavlja se upload id dobijen kao rezultat POST zahteva
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

        reply = manager->put(requestPUT, fileDrive);
        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                qDebug() << "SUCCESS";
            }
            fileDrive->close();
            delete  fileDrive;
            emit endConnectSignal();
        });
    }
    else
        qDebug() << "error: " << reply->errorString();
}
