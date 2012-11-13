//------------------------------------------------------------------------------
#ifndef AUTORESIZINGTEXTITEM_H
#define AUTORESIZINGTEXTITEM_H
//------------------------------------------------------------------------------
#include <QGraphicsObject>
#include <QFont>
#include <QString>
#include <QPen>
//------------------------------------------------------------------------------
class AutoResizingTextItem : public QGraphicsObject {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit AutoResizingTextItem(QGraphicsItem *parent = 0);
		~AutoResizingTextItem();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		const QString & text() const;
		const QFont & font() const;
		const QPen & pen() const;
		bool isVisible() const;

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
	public slots:
		void setSize(qreal w, qreal h);
		void setSize(const QSizeF & size);
		void setRect(const QRectF &rect);
		void setVisible(bool visible);
		void setText(const QString &text);
		void setFont(const QFont &font);
		void setPen(const QPen &pen);

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
	private:
		void adjustText();

//------------------------------------------------------------------------------
// Members
//------------------------------------------------------------------------------
	private:
		QSizeF	m_size;
		bool		m_visible;
		QString	m_text;
		QFont		m_font;
		QPen		m_pen;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // AUTORESIZINGTEXTITEM_H
//------------------------------------------------------------------------------
