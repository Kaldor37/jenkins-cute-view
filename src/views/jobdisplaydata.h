//------------------------------------------------------------------------------
#ifndef JOBDISPLAYDATA_H
#define JOBDISPLAYDATA_H
//------------------------------------------------------------------------------
#include <QString>
#include <time.h>
//------------------------------------------------------------------------------
class JobDisplayData{
	public:
		enum eJobStatus{
			StatusInactiveOrNeverBuilt = 0,
			StatusLastBuildSuccessful,
			StatusLastBuildSuccessfulButUnstable,
			StatusLastBuildFailed
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
		time_t				getStartTime() const;
		ulong					getEstimatedDuration() const;

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
		void setStartTime(time_t value);
		void setEstimatedDuration(ulong value);
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
		time_t		m_startTime;
		ulong			m_estimatedDuration;

//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // JOBDISPLAYDATA_H
//------------------------------------------------------------------------------
