//------------------------------------------------------------------------------
#ifndef JENKINSGRAPHICSVIEW_H
#define JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
#include <QGraphicsView>
//------------------------------------------------------------------------------
class JenkinsGraphicsScene;
//------------------------------------------------------------------------------
class JenkinsGraphicsView : public QGraphicsView {
	Q_OBJECT

	public:
		explicit JenkinsGraphicsView(QWidget *parent = 0);
		~JenkinsGraphicsView();

	signals:

	protected:
		void resizeEvent(QResizeEvent *event);

	public slots:

	private:
		JenkinsGraphicsScene *m_scene;
};
//------------------------------------------------------------------------------
class JenkinsGraphicsScene : public QGraphicsScene {
	Q_OBJECT

	public:
		explicit JenkinsGraphicsScene(QWidget *parent = 0);
		~JenkinsGraphicsScene();
};
//------------------------------------------------------------------------------
#endif // JENKINSGRAPHICSVIEW_H
//------------------------------------------------------------------------------
