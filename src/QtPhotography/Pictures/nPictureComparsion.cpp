#include <qtphotography.h>
#include "ui_nPictureComparsion.h"

N::PictureComparsion:: PictureComparsion ( QWidget * parent , Plan * p )
                     : Widget            (           parent ,        p )
                     , ui                ( new Ui::nPictureComparsion  )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::PictureComparsion::~PictureComparsion(void)
{
  delete ui ;
}

void N::PictureComparsion::Configure(void)
{
  ui   -> setupUi ( this               ) ;
  plan -> setFont ( this               ) ;
  setAcceptDrops  ( true               ) ;
  setDropFlag     ( DropPicture , true ) ;
  ////////////////////////////////////////
  ui->Comparsion->addItem(tr("Shapes"))  ;
  ////////////////////////////////////////
  ui->Algorithm->addItem(tr("I1"))       ;
  ui->Algorithm->addItem(tr("I2"))       ;
  ui->Algorithm->addItem(tr("I3"))       ;
}

void N::PictureComparsion::focusInEvent(QFocusEvent * event)
{
  if (!focusIn (event)) QWidget::focusInEvent (event) ;
}

void N::PictureComparsion::focusOutEvent(QFocusEvent * event)
{
  if (!focusOut(event)) QWidget::focusOutEvent(event) ;
}

void N::PictureComparsion::resizeEvent(QResizeEvent * event)
{
  if (Relocation()) event->accept() ;
  else QWidget::resizeEvent(event)  ;
}

void N::PictureComparsion::showEvent(QShowEvent * event)
{
  QWidget::showEvent(event) ;
  Relocation        (     ) ;
}

void N::PictureComparsion::dragEnterEvent(QDragEnterEvent * event)
{
  if (dragEnter(event)) event->acceptProposedAction() ; else {
    if (PassDragDrop) QWidget::dragEnterEvent(event)         ;
    else event->ignore()                                     ;
  }                                                          ;
}

void N::PictureComparsion::dragLeaveEvent(QDragLeaveEvent * event)
{
  if (removeDrop()) event->accept() ; else           {
    if (PassDragDrop) QWidget::dragLeaveEvent(event) ;
    else event->ignore()                             ;
  }                                                  ;
}

void N::PictureComparsion::dragMoveEvent(QDragMoveEvent * event)
{
  if (dragMove(event)) event->acceptProposedAction() ; else {
    if (PassDragDrop) QWidget::dragMoveEvent(event)         ;
    else event->ignore()                                    ;
  }                                                         ;
}

void N::PictureComparsion::dropEvent(QDropEvent * event)
{
  if (drop(event)) event->acceptProposedAction() ; else {
    if (PassDragDrop) QWidget::dropEvent(event)         ;
    else event->ignore()                                ;
  }                                                     ;
}

bool N::PictureComparsion::acceptDrop(QWidget * source,const QMimeData * mime)
{
  return true ;
}

bool N::PictureComparsion::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return true ;
}

bool N::PictureComparsion::dropMoving(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return true ;
}

bool N::PictureComparsion::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return dropItems ( source , mime , pos ) ;
}

bool N::PictureComparsion::Relocation(void)
{
  int h = height()                     ;
  ui -> First  -> resize ( h     , h ) ;
  ui -> Second -> resize ( h     , h ) ;
  ui -> Second -> move   ( h+120 , 0 ) ;
  return true                          ;
}

bool N::PictureComparsion::dropPictures(QWidget * source,QPointF pos,const UUIDs & uuids)
{ Q_UNUSED ( source )               ;
  Q_UNUSED ( pos    )               ;
  QList<QLabel *> Labels            ;
  QSize S(height(),height())        ;
  SUID uuid                         ;
  Labels << ui->First               ;
  Labels << ui->Second              ;
  PictureManager PM(plan)           ;
  foreach (uuid,uuids)              {
    QImage * I = PM.Picture(uuid)   ;
    if (NotNull(I))                 {
      Images << *I                  ;
      Uuids  << uuid                ;
      delete I                      ;
    }                               ;
  }                                 ;
  while (Uuids.count()>2)           {
    Uuids  . takeAt ( 0 )           ;
    Images . takeAt ( 0 )           ;
  }                                 ;
  for (int i=0;i<Uuids.count();i++) {
    QImage  I                       ;
    QPixmap P                       ;
    I = PM.Icon(Images[i],S)        ;
    P = QPixmap::fromImage(I)       ;
    uuid = Uuids[i]                 ;
    Labels[i]->setPixmap(P)         ;
  }                                 ;
  if (Uuids.count()>=2) Compare ( ) ;
  Alert ( Done )                    ;
  return true                       ;
}

void N::PictureComparsion::Options(int)
{
  Compare ( ) ;
}

bool N::PictureComparsion::Compare(void)
{
  nKickOut ( Uuids  . count () < 2 , false )                ;
  nKickOut ( Images . count () < 2 , false )                ;
  int Method    = ui->Comparsion->currentIndex()            ;
  int Algorithm = ui->Algorithm ->currentIndex()            ;
  nKickOut ( Method !=0 , false )                           ;
  PictureManager PM(plan)                                   ;
  int        CM                                             ;
  switch ( Algorithm )                                      {
    case 0: CM = CV_CONTOURS_MATCH_I1 ; break               ;
    case 1: CM = CV_CONTOURS_MATCH_I2 ; break               ;
    case 2: CM = CV_CONTOURS_MATCH_I3 ; break               ;
    default                                                 :
      CM = CV_CONTOURS_MATCH_I1                             ;
    break                                                   ;
  }                                                         ;
  double similarity = PM.Similarity(Images[0],Images[1],CM) ;
  QString T                                                 ;
  T = QString("%1").arg(similarity,0,'f',6,QChar('0'))      ;
  ui->Result->setText(T)                                    ;
  return true                                               ;
}
