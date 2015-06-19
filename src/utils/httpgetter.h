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
#include <functional>
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
		HttpGetter(const HttpGetter &) = delete;
		HttpGetter & operator=(const HttpGetter &) = delete;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public:
		typedef std::function<void(const QString &data, QNetworkReply::NetworkError errCode, const QString &errString)> getCallback;

		template <typename Listener, typename Method>
		static getCallback bindGetCallback(Listener l, Method m){
			return std::bind(m, l, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		}

	public slots:
		void get(const QUrl &url, getCallback function);
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
