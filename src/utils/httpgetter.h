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
// Public functions
//------------------------------------------------------------------------------
	public:
		template <typename Listener, typename Method>
		void get(const QUrl &url, Listener l, Method m){
			get(url, std::bind(m, l, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		}

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setBasicAuthorization(const QString &user, const QString &pass);

//------------------------------------------------------------------------------
// Private functions/slots/typedefs
//------------------------------------------------------------------------------
	private slots:
		void networkReply_finished();

	private:
		typedef std::function<void(const QString &data, QNetworkReply::NetworkError errCode, const QString &errString)> getCallback;

		void get(const QUrl &url, getCallback function);

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
