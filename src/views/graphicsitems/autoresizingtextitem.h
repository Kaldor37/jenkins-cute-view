//------------------------------------------------------------------------------
#ifndef AUTORESIZINGTEXTITEM_H
#define AUTORESIZINGTEXTITEM_H
//------------------------------------------------------------------------------
#include <QGraphicsObject>
#include <QGraphicsLayoutItem>
#include <QFont>
#include <QString>
#include <QPen>
//------------------------------------------------------------------------------
class AutoResizingTextItem : public QGraphicsObject, public QGraphicsLayoutItem {
	Q_OBJECT
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
	public:
		explicit AutoResizingTextItem(QGraphicsItem *parent = nullptr);
		~AutoResizingTextItem();

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
	public:
		QRectF	boundingRect() const;
		QSizeF	sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
		void		paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

		const		QString & text() const;
		const		QFont & font() const;
		const		QPen & pen() const;
		bool		isVisible() const;
		bool		isShadowed() const;
		int		textFlags() const;

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
		void setFontFamily(const QString &fontFamily);
		void setPen(const QPen &pen);
		void setShadowed(bool value);
		void setTextFlags(int flags);

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
		bool		m_shadowed;
		uint		m_shadowOffset;
		int		m_textFlags;

		const static int shadowOffsetFontHeightFactor = 20;
//------------------------------------------------------------------------------
};
//------------------------------------------------------------------------------
#endif // AUTORESIZINGTEXTITEM_H
//------------------------------------------------------------------------------
