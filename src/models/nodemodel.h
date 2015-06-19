//------------------------------------------------------------------------------
#ifndef NODEMODEL_H
#define NODEMODEL_H
//------------------------------------------------------------------------------
#include <QString>
#include <QObject>
//------------------------------------------------------------------------------
class NodeModel : public QObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructors/Destructor
//------------------------------------------------------------------------------
	private:
		NodeModel() = delete;
		NodeModel(const NodeModel &src) = delete;
		NodeModel & operator=(const NodeModel &src) = delete;

	public:
		explicit NodeModel(QObject *parent=nullptr);
		~NodeModel();

//------------------------------------------------------------------------------
// Members declaration
//------------------------------------------------------------------------------
#define NODE_DECLARE_MEMBER(Type, Member, Init) \
	public: \
		const Type & get##Member() const; \
	public slots: \
		void set##Member(const Type & Member); \
	private: \
		Type m_##Member = Init
//------------------------------------------------------------------------------
// General
NODE_DECLARE_MEMBER(QString,	DisplayName,			QString());
NODE_DECLARE_MEMBER(bool,		Idle,						false);
NODE_DECLARE_MEMBER(uint,		NumExecutors,			0);
NODE_DECLARE_MEMBER(bool,		Offline,					false);
NODE_DECLARE_MEMBER(QString,	OfflineCauseReason,	QString());
NODE_DECLARE_MEMBER(bool,		TemporarilyOffline,	false);
//------------------------------------------------------------------------------
#undef NODE_DECLARE_MEMBER
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // NODEMODEL_H
//------------------------------------------------------------------------------
