//------------------------------------------------------------------------------
#ifndef NODEGRAPHICSITEM_H
#define NODEGRAPHICSITEM_H
//------------------------------------------------------------------------------
#include <QGraphicsObject>
//------------------------------------------------------------------------------
class QSizeF;
class AutoResizingTextItem;
//------------------------------------------------------------------------------
class NodeGraphicsItem : public QGraphicsObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit NodeGraphicsItem(QGraphicsItem *parent = nullptr);
		~NodeGraphicsItem();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		const QString & getName() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setSize(qreal w, qreal h);
		void setSize(const QSizeF & size);
		void setRect(const QRectF &rect);

		void setName(const QString &name);
		void setColor(const QColor &color);

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void updateLayout();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QSizeF	m_size;
		QString	m_name;
		QColor	m_color;
		AutoResizingTextItem	*m_nameItem;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // NODEGRAPHICSITEM_H
//------------------------------------------------------------------------------
