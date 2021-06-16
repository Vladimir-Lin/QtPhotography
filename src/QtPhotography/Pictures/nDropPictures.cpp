#include <qtphotography.h>

N::DropPictures:: DropPictures ( QWidget * parent , Plan * p )
                : ComboBox     (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::DropPictures::~DropPictures(void)
{
}

void N::DropPictures::Configure(void)
{
  setAcceptDrops     ( true               ) ;
  setDropFlag        ( DropPicture , true ) ;
  setEditable        ( true               ) ;
  setInsertPolicy    ( NoInsert           ) ;
  setMaxVisibleItems ( 30                 ) ;
  DoProcessEvents                           ;
  ///////////////////////////////////////////
  int w = plan -> screen.widthPixels ( 55 ) ;
  setMinimumWidth    ( w                  ) ;
  setMaximumWidth    ( w                  ) ;
  ///////////////////////////////////////////
  plan -> setFont    ( this               ) ;
}

bool N::DropPictures::acceptDrop(QWidget * source,const QMimeData * mime)
{
  return dropHandler ( mime ) ;
}

bool N::DropPictures::dropNew(QWidget * source,const QMimeData * mime,QPoint pos)
{ Q_UNUSED ( source )                                                      ;
  Q_UNUSED ( pos    )                                                      ;
  QString mtype                                                            ;
  mtype = AbstractGui :: MimeType ( mime , "picture/uuid;picture/uuids;" ) ;
  if ( ( "picture/uuid" == mtype ) || ( mtype == "picture/uuids" )       ) {
    UUIDs      Uuids                                                       ;
    QByteArray data = mime -> data ( mtype )                               ;
    if (data.size()>0)                                                     {
      if ( "picture/uuid"  == mtype ) Uuids << GetUuid  ( data )      ; else
      if ( "picture/uuids" == mtype ) Uuids  = GetUuids ( data )           ;
    }                                                                      ;
    plan -> showMessage (  tr    ( "do %1 on %2 pictures"                  )
                           . arg ( currentText()                           )
                           . arg ( Uuids.count()                       ) ) ;
    return true                                                            ;
  }                                                                        ;
  return false                                                             ;
}

bool N::DropPictures::dropAppend(QWidget * source,const QMimeData * mime,QPoint pos)
{
  return dropItems ( source , mime , pos ) ;
}

bool N::DropPictures::dropPictures(QWidget * widget,QPointF pos,const UUIDs & Uuids)
{ Q_UNUSED ( widget   ) ;
  Q_UNUSED ( pos      ) ;
  UUIDs U = Uuids       ;
  SUID  u = toUuid  ( ) ;
  emit Effect ( u , U ) ;
  return true           ;
}
