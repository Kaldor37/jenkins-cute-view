//------------------------------------------------------------------------------
#ifndef JOBDISPLAYDATA_H
#define JOBDISPLAYDATA_H
//------------------------------------------------------------------------------
#include <QString>
//------------------------------------------------------------------------------
class JobDisplayData{
	public:
		enum eJobStatus{
			StatusNeverBuilt = 0,
			StatusLastBuildSuccessful,
			StatusLastBuildSuccessfulButUnstable,
			StatusLastBuildFailed,
			StatusInactiveOrAborted
		};

//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		JobDisplayData();
		JobDisplayData(const JobDisplayData &);
		JobDisplayData & operator=(const JobDisplayData &);
		~JobDisplayData();

//------------------------------------------------------------------------------
// Getters
//------------------------------------------------------------------------------
	public:
		const QString &	getName() const;
		eJobStatus			getStatus() const;
		uint					getStability() const;
		uint					getLastBuildNum() const;
		const QString &	getLastBuildDesc() const;
		bool					isRunning() const;
		qint64				getStartTime() const;
		qint64				getEstimatedDuration() const;
		uint					getPositionInQueue() const;

//------------------------------------------------------------------------------
// Setters
//------------------------------------------------------------------------------
	public:
		void setName(const QString &value);
		void setStatus(eJobStatus value);
		void setStability(uint value);
		void setLastBuildNum(uint value);
		void setLastBuildDesc(const QString &value);
		void setRunning(bool value);
		void setStartTime(qint64 value);
		void setEstimatedDuration(qint64 value);
		void setPositionInQueue(uint value);
//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QString		m_name;
		eJobStatus	m_status;
		uint			m_stability;
		uint			m_lastBuildNum;
		QString		m_lastBuildDesc;
		bool			m_running;
		qint64		m_startTime;
		qint64		m_estimatedDuration;
		uint			m_positionInQueue;

//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JOBDISPLAYDATA_H
//------------------------------------------------------------------------------
