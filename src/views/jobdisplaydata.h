//------------------------------------------------------------------------------
#ifndef JOBDISPLAYDATA_H
#define JOBDISPLAYDATA_H
//------------------------------------------------------------------------------
#include <QString>
//------------------------------------------------------------------------------
class JobDisplayData{
	public:
		enum class JobStatus {
			NeverBuilt,
			LastBuildSuccessful,
			LastBuildSuccessfulButUnstable,
			LastBuildFailed,
			InactiveOrAborted
		};

//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		JobDisplayData() = default;
		JobDisplayData(const JobDisplayData &) = default;
		JobDisplayData & operator=(const JobDisplayData &) = default;
		~JobDisplayData(){}

//------------------------------------------------------------------------------
// Getters
//------------------------------------------------------------------------------
	public:
		const QString &	getName() const;
		JobStatus			getStatus() const;
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
		void setStatus(JobStatus value);
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
		JobStatus	m_status = JobStatus::NeverBuilt;
		uint			m_stability = 0;
		uint			m_lastBuildNum = 0;
		QString		m_lastBuildDesc;
		bool			m_running = false;
		qint64		m_startTime = 0;
		qint64		m_estimatedDuration = 0;
		uint			m_positionInQueue = 0;

//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JOBDISPLAYDATA_H
//------------------------------------------------------------------------------
