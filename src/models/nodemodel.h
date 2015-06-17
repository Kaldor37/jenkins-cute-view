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
		NodeModel();
		NodeModel(const NodeModel &src);
		NodeModel & operator=(const NodeModel &src);

	public:
		explicit NodeModel(QObject *parent=nullptr);
		~NodeModel();

//------------------------------------------------------------------------------
// Members declaration
//------------------------------------------------------------------------------
#define NODE_DECLARE_MEMBER(Type, Member) \
	public: \
		const Type & get##Member() const; \
	public slots: \
		void set##Member(const Type & Member); \
	private: \
		Type m_##Member;
//------------------------------------------------------------------------------
// General
NODE_DECLARE_MEMBER(QString,	DisplayName)
NODE_DECLARE_MEMBER(bool,		Idle)
NODE_DECLARE_MEMBER(uint,		NumExecutors)
NODE_DECLARE_MEMBER(bool,		Offline)
NODE_DECLARE_MEMBER(QString,	OfflineCauseReason)
NODE_DECLARE_MEMBER(bool,		TemporarilyOffline)
//------------------------------------------------------------------------------
#undef NODE_DECLARE_MEMBER
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // NODEMODEL_H
//------------------------------------------------------------------------------
