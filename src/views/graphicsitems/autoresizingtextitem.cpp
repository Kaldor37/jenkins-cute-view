//------------------------------------------------------------------------------
#include "autoresizingtextitem.h"

#include <QPainter>
#include <QDebug>
//------------------------------------------------------------------------------
// Constructor(s)/Destructor
//------------------------------------------------------------------------------
AutoResizingTextItem::AutoResizingTextItem(QGraphicsItem *parent/* = 0*/):QGraphicsObject(parent), QGraphicsLayoutItem(),
	m_visible(true),
	m_shadowed(false),
	m_textFlags(Qt::AlignCenter){
	setGraphicsItem(this);
}
//------------------------------------------------------------------------------
AutoResizingTextItem::~AutoResizingTextItem(){ }
//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
QRectF AutoResizingTextItem::boundingRect() const{
	return (m_visible)?QRectF(0,0,m_size.width(), m_size.height()):QRectF();
}
//------------------------------------------------------------------------------
QSizeF AutoResizingTextItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint/* = QSizeF()*/) const {
	Q_UNUSED(which);
	return constraint;
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option);
	Q_UNUSED(widget);

	if(!m_visible)
		return;
	if(m_text.isEmpty())
		return;
	if(m_size.isEmpty() || m_size.isNull())
		return;

	painter->save();

	painter->setFont(m_font);

	QRect rect = boundingRect().toRect();

	if(m_shadowed){
		QPen shadowPen(m_pen);
		shadowPen.setColor(Qt::black);
		painter->setPen(shadowPen);

		QRect shadowRect(rect);
		shadowRect.setX(shadowRect.x()+m_shadowOffset);
		shadowRect.setY(shadowRect.y()+m_shadowOffset);

		painter->drawText(shadowRect, m_textFlags, m_text);
	}

	painter->setPen(m_pen);
	painter->drawText(rect, m_textFlags, m_text);

	painter->restore();
}
//------------------------------------------------------------------------------
const QString & AutoResizingTextItem::text() const{
	return m_text;
}
//------------------------------------------------------------------------------
const QFont & AutoResizingTextItem::font() const{
	return m_font;
}
//------------------------------------------------------------------------------
const QPen & AutoResizingTextItem::pen() const{
	return m_pen;
}
//------------------------------------------------------------------------------
bool AutoResizingTextItem::isVisible() const{
	return m_visible;
}
//------------------------------------------------------------------------------
bool AutoResizingTextItem::isShadowed() const{
	return m_shadowed;
}
//------------------------------------------------------------------------------
int AutoResizingTextItem::textFlags() const{
	return m_textFlags;
}
//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------
void AutoResizingTextItem::setSize(qreal w, qreal h){
	m_size = QSizeF(w, h);
	adjustText();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setSize(const QSizeF & size){
	m_size = size;
	adjustText();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setRect(const QRectF &rect){
	setPos(rect.x(), rect.y());
	m_size = rect.size();
	adjustText();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setVisible(bool visible){
	m_visible = visible;
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setShadowed(bool value){
	m_shadowed = value;
	adjustText();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setText(const QString &text){
	m_text = text;
	adjustText();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setFont(const QFont &font){
	m_font = font;
	adjustText();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setPen(const QPen &pen){
	m_pen = pen;
	update();
}
//------------------------------------------------------------------------------
void AutoResizingTextItem::setTextFlags(int flags){
	m_textFlags = flags;
	update();
}
//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
void AutoResizingTextItem::adjustText(){
	if(m_size.isEmpty() || m_size.isNull())
		return;
	if(m_text.isEmpty())
		return;

	const qreal maxWidth = m_size.width();
	const qreal maxHeight = m_size.height();

	QFontMetrics fm(m_font);
	QFont resizedFont(m_font);

	// Too big, reduce
	if(fm.height() > m_size.height() || fm.width(m_text) > m_size.width()){
		if(resizedFont.pointSize() > 1){
			do{
				resizedFont.setPointSize(resizedFont.pointSize()-1);
				fm = QFontMetrics(resizedFont);
			}while((fm.width(m_text) > maxWidth || fm.height() > maxHeight) && resizedFont.pointSize() > 1);
			m_font = resizedFont;
		}
	}
	// Fits or too small, try to expand
	else{
		QFont lastFont;
		do{
			lastFont = resizedFont;
			resizedFont.setPointSize(resizedFont.pointSize()+1);
			fm = QFontMetrics(resizedFont);
		}while(fm.width(m_text) <= maxWidth && fm.height() <= maxHeight);
		m_font = lastFont;
	}

	if(m_shadowed)
		m_shadowOffset = QFontMetrics(m_font).height()/shadowOffsetFontHeightFactor;

	update();
}
//------------------------------------------------------------------------------
