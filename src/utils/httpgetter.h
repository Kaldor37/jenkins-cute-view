//------------------------------------------------------------------------------
#ifndef HTTPGETTER_H
#define HTTPGETTER_H
//------------------------------------------------------------------------------
#include <QObject>
#include <QUrl>
#include <QNetworkReply>
#include <QSslError>
#include <QList>
#include <QMap>
//------------------------------------------------------------------------------
#define httpGetter	HttpGetter::instance()
//------------------------------------------------------------------------------
class QNetworkAccessManager;
class QString;
class NetworkReplyManager;
//------------------------------------------------------------------------------
class HttpGetter : public QObject{
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		static void create();
		static void destroy();
		static HttpGetter & instance();

		~HttpGetter();

	private:
		HttpGetter(QObject *parent = nullptr);
		HttpGetter(const HttpGetter &);
		HttpGetter & operator=(const HttpGetter &);

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void get(const QUrl &url, QObject *listener, const char *finishedSlot);
		void setBasicAuthorization(const QString &user, const QString &pass);

	private slots:
		void networkReply_finished();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		static HttpGetter * m_instance;

		QNetworkAccessManager *	m_networkAccessManager;
		QMap<QNetworkReply*,NetworkReplyManager*> m_replyManagers;
		QByteArray m_authorizationHeader;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
class NetworkReplyManager : public QObject {
	Q_OBJECT

	public:
		explicit NetworkReplyManager(QNetworkReply *networkReply, QObject *parent=nullptr);
		~NetworkReplyManager();

	signals:
		void finished(QString data, QNetworkReply::NetworkError errorCode, QString errString);

	private slots:
		void networkReply_error(QNetworkReply::NetworkError error);
		void networkReply_sslErrors(QList<QSslError> errors);
		void networkReply_finished();

	private:
		QNetworkReply *m_networkReply;
};
//------------------------------------------------------------------------------
#endif // HTTPGETTER_H
//------------------------------------------------------------------------------
