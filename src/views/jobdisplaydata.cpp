//------------------------------------------------------------------------------
#include "views/jobdisplaydata.h"
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
JobDisplayData::JobDisplayData():
	m_name(),
	m_status(StatusNeverBuilt),
	m_stability(0),
	m_lastBuildNum(0),
	m_lastBuildDesc(),
	m_running(false),
	m_startTime(0),
	m_estimatedDuration(0),
	m_positionInQueue(0)
{}
//------------------------------------------------------------------------------
JobDisplayData::JobDisplayData(const JobDisplayData &src){
	m_name = src.m_name;
	m_status = src.m_status;
	m_stability = src.m_stability;
	m_lastBuildNum = src.m_lastBuildNum;
	m_lastBuildDesc = src.m_lastBuildDesc;
	m_running = src.m_running;
	m_startTime = src.m_startTime;
	m_estimatedDuration = src.m_estimatedDuration;
	m_positionInQueue = src.m_positionInQueue;
}
//------------------------------------------------------------------------------
JobDisplayData & JobDisplayData::operator=(const JobDisplayData &src){
	if(&src != this){
		m_name = src.m_name;
		m_status = src.m_status;
		m_stability = src.m_stability;
		m_lastBuildNum = src.m_lastBuildNum;
		m_lastBuildDesc = src.m_lastBuildDesc;
		m_running = src.m_running;
		m_startTime = src.m_startTime;
		m_estimatedDuration = src.m_estimatedDuration;
		m_positionInQueue = src.m_positionInQueue;
	}
	return *this;
}
//------------------------------------------------------------------------------
JobDisplayData::~JobDisplayData(){}
//------------------------------------------------------------------------------
//  JobDisplayData::getters
//------------------------------------------------------------------------------
const QString & JobDisplayData::JobDisplayData::getName() const {
	return m_name;
}
//------------------------------------------------------------------------------
JobDisplayData::eJobStatus JobDisplayData::getStatus() const {
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
void JobDisplayData::setStatus(JobDisplayData::eJobStatus value){
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
