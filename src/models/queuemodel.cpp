//------------------------------------------------------------------------------
#include "queuemodel.h"
#include <QDebug>
//------------------------------------------------------------------------------
#define QUEUE_INIT_INT_MEMBER(Member) m_##Member(0)
#define QUEUE_INIT_STR_MEMBER(Member) m_##Member()

#define QUEUE_IMPL_MEMBER(Type, Member) \
	const Type & QueueModel::get##Member() const { \
		return m_##Member; \
	} \
	void QueueModel::set##Member(const Type & value){ \
		m_##Member = value; \
	}
//------------------------------------------------------------------------------
QueueModel::QueueModel(QObject *parent/*=0*/):QObject(parent)
{}
//------------------------------------------------------------------------------
QueueModel::~QueueModel(){
}
//------------------------------------------------------------------------------
// TODO
//------------------------------------------------------------------------------
#undef QUEUE_INIT_INT_MEMBER
#undef QUEUE_INIT_STR_MEMBER
#undef QUEUE_IMPL_MEMBER
//------------------------------------------------------------------------------
