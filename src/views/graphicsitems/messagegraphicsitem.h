//------------------------------------------------------------------------------
#ifndef MESSAGEGRAPHICSITEM_H
#define MESSAGEGRAPHICSITEM_H
//------------------------------------------------------------------------------
#include <QGraphicsObject>
#include <QColor>

class AutoResizingTextItem;
//------------------------------------------------------------------------------
class MessageGraphicsItem : public QGraphicsObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Enum(s)
//------------------------------------------------------------------------------
	public:
		enum eMessageType {
			NoType = 0,
			Normal,
			Warning,
			Error
		};
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit MessageGraphicsItem(QGraphicsItem *parent = nullptr);
		~MessageGraphicsItem();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setSize(qreal w, qreal h);
		void setSize(const QSizeF & size);
		void setRect(const QRectF &rect);

		void setMessage(eMessageType, const QString &);

//------------------------------------------------------------------------------
// Private function
//------------------------------------------------------------------------------
	private:
		void updateMessage();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QSizeF						m_size;
		AutoResizingTextItem		*m_messageItem;
		QColor						m_backgroundColor;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // MESSAGEGRAPHICSITEM_H
//------------------------------------------------------------------------------
