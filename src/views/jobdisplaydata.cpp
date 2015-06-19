//------------------------------------------------------------------------------
#include "views/jobdisplaydata.h"
//------------------------------------------------------------------------------
//  JobDisplayData::getters
//------------------------------------------------------------------------------
const QString & JobDisplayData::JobDisplayData::getName() const {
	return m_name;
}
//------------------------------------------------------------------------------
JobDisplayData::JobStatus JobDisplayData::getStatus() const {
	return m_status;
}
//------------------------------------------------------------------------------
uint JobDisplayData::getStability() const {
	return m_stability;
}
//------------------------------------------------------------------------------
uint JobDisplayData::getLastBuildNum() const {
	return m_lastBuildNum;
}
//------------------------------------------------------------------------------
const QString & JobDisplayData::getLastBuildDesc() const {
	return m_lastBuildDesc;
}
//------------------------------------------------------------------------------
bool JobDisplayData::isRunning() const {
	return m_running;
}
//------------------------------------------------------------------------------
qint64 JobDisplayData::getStartTime() const {
	return m_startTime;
}
//------------------------------------------------------------------------------
qint64 JobDisplayData::getEstimatedDuration() const {
	return m_estimatedDuration;
}
//------------------------------------------------------------------------------
uint JobDisplayData::getPositionInQueue() const {
	return m_positionInQueue;
}
//------------------------------------------------------------------------------
// Setters
//------------------------------------------------------------------------------
void JobDisplayData::setName(const QString &value){
	m_name = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setStatus(JobDisplayData::JobStatus value){
	m_status = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setStability(uint value){
	m_stability = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setLastBuildDesc(const QString &value){
	m_lastBuildDesc = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setLastBuildNum(uint value){
	m_lastBuildNum = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setRunning(bool value){
	m_running = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setStartTime(qint64 value){
	m_startTime = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setEstimatedDuration(qint64 value){
	m_estimatedDuration = value;
}
//------------------------------------------------------------------------------
void JobDisplayData::setPositionInQueue(uint value){
	m_positionInQueue = value;
}
//------------------------------------------------------------------------------
