//------------------------------------------------------------------------------
#include "utils/httpgetter.h"
#include "application.h"

#include <QDebug>
#include <QNetworkRequest>
//------------------------------------------------------------------------------
HttpGetter* HttpGetter::m_instance(0);
//------------------------------------------------------------------------------
// Constructor(s)/Destructor/Singleton static funcs
//------------------------------------------------------------------------------
void HttpGetter::create(){
	Q_ASSERT(m_instance == 0);
	m_instance = new HttpGetter();
}
//------------------------------------------------------------------------------
void HttpGetter::destroy(){
	Q_ASSERT(m_instance);
	delete m_instance;
}
//------------------------------------------------------------------------------
HttpGetter&  HttpGetter::instance(){
	Q_ASSERT(m_instance);
	return *m_instance;
}
//------------------------------------------------------------------------------
HttpGetter::HttpGetter(QObject *parent) : QObject(parent){
	m_networkAccessManager = new QNetworkAccessManager(this);
}
//------------------------------------------------------------------------------
HttpGetter::~HttpGetter(){}
//------------------------------------------------------------------------------
NetworkReplyManager::NetworkReplyManager(QNetworkReply *networkReply, QObject *parent) : QObject(parent),
	m_networkReply(networkReply)
{
	connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),	SLOT(networkReply_error(QNetworkReply::NetworkError)));
	connect(networkReply, SIGNAL(sslErrors(QList<QSslError>)),			SLOT(networkReply_sslErrors(QList<QSslError>)));
	connect(networkReply, SIGNAL(finished()),									SLOT(networkReply_finished()));
}
//------------------------------------------------------------------------------
NetworkReplyManager::~NetworkReplyManager(){}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void HttpGetter::get(const QUrl &url, QObject *listener, const char *finishedSlot){
	if(App.verbose())
		qDebug()<<"HttpGetter::get("<<url<<")";

	QNetworkRequest netReq(url);
	if(!m_authorizationHeader.isEmpty()){
		netReq.setRawHeader("Authorization", m_authorizationHeader);
	}

	QNetworkReply *netRep = m_networkAccessManager->get(netReq);
	netRep->ignoreSslErrors();

	NetworkReplyManager *manager = new NetworkReplyManager(netRep, this);
	QObject::connect(manager, SIGNAL(finished(QString,QNetworkReply::NetworkError,QString)), listener, finishedSlot);

	// Connect on finished for deletion
	connect(netRep, SIGNAL(finished()), SLOT(networkReply_finished()), Qt::QueuedConnection);

	m_replyManagers[netRep] = manager;
}
//------------------------------------------------------------------------------
void HttpGetter::setBasicAuthorization(const QString &user, const QString &pass){
	if(user.isEmpty() || pass.isEmpty()){
		m_authorizationHeader.clear();
		return;
	}

	m_authorizationHeader = "Basic " + QString("%1:%2").arg(user).arg(pass).toLocal8Bit().toBase64();
}
//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------
void HttpGetter::networkReply_finished(){
	QNetworkReply *netRep = qobject_cast<QNetworkReply*>(QObject::sender());
	if(netRep){
		if(m_replyManagers.contains(netRep)){
			NetworkReplyManager * manager = m_replyManagers.take(netRep);
			manager->deleteLater();
		}
		netRep->deleteLater();
	}
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void NetworkReplyManager::networkReply_error(QNetworkReply::NetworkError error){
	qWarning()<<"NetworkReplyManager::networkReply_error("<<m_networkReply->url().toString()<<") : "<<error;
}
//------------------------------------------------------------------------------
void NetworkReplyManager::networkReply_sslErrors(QList<QSslError> errors){
	uint i=1;
	foreach(const QSslError &error, errors){
		qWarning()<<"NetworkReplyManager::networkReply_sslErrors("<<m_networkReply->url().toString()<<") : Err "<<i<<" : "<<error.errorString();
		++i;
	}
}
//------------------------------------------------------------------------------
void NetworkReplyManager::networkReply_finished(){
	QNetworkReply::NetworkError err = m_networkReply->error();
	QString content;

	if(err == QNetworkReply::NoError)
		content = m_networkReply->readAll();

	if(App.verbose())
		qDebug()<<"NetworkReplyManager::networkReply_finished("<<m_networkReply->url()<<") - Error : "<<m_networkReply->error();

	emit finished(content, m_networkReply->error(), m_networkReply->errorString());
}
//------------------------------------------------------------------------------
