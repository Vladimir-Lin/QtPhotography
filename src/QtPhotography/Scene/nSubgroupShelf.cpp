#include <qtphotography.h>

#ifdef QT_OPENGL_LIB

N::SubgroupShelf:: SubgroupShelf ( QObject * parent , Plan * p )
                 : ViewShelf     (           parent ,        p )
{
  WidgetClass   ;
  Configure ( ) ;
}

N::SubgroupShelf:: SubgroupShelf ( QObject * parent , Plan * p , SceneDestructor * des )
                 : ViewShelf     (           parent ,        p ,                   des )
{
  Configure ( ) ;
}

N::SubgroupShelf::~SubgroupShelf (void)
{
}

void N::SubgroupShelf::Configure(void)
{
  QObject::setProperty ( "Type"             , "SubgroupShelf"      ) ;
  setVariable          ( "Type"             , "SubgroupShelf"      ) ;
  setVariable          ( "Menu"             , true                 ) ;
  ////////////////////////////////////////////////////////////////////
  setVariable          ( "Access"           , "First"              ) ;
  setVariable          ( "T1"               , Types::Subgroup      ) ;
  setVariable          ( "T2"               , Types::Subgroup      ) ;
  setVariable          ( "T3"               , Types::Subgroup      ) ;
  setVariable          ( "T4"               , Types::Subgroup      ) ;
  setVariable          ( "T5"               , Types::Subgroup      ) ;
  setVariable          ( "Sorting"          , Qt::AscendingOrder   ) ;
  setVariable          ( "AutoRefresh"      , true                 ) ;
  setVariable          ( "ReplaceIcon"      , ":/images/model.png" ) ;
  setVariable          ( "ArrangeBy"        , "Icon"               ) ;
  setVariable          ( "ScrollBy"         , "Start"              ) ;
}

int N::SubgroupShelf::Indexing(RangeBar * rbu,int direction,QVariants & v)
{
//  qint64 s = v [ 0 ] . toLongLong ( )     ; // start
//  qint64 e = v [ 1 ] . toLongLong ( )     ; // end
//  qint64 p = v [ 2 ] . toLongLong ( )     ; // page
  qint64 i = v [ 3 ] . toLongLong ( )     ; // index
  UUIDs  U = CurrentItems                 ;
  setVariable  ( "CurrentPageIndex" , i ) ;
  PickItem::ArrangeItems ( i , U )        ;
  return 0                                ;
}

bool N::SubgroupShelf::Relocation(QRectF viewport)
{
  return CentralRelocation ( viewport ) ;
}

bool N::SubgroupShelf::Menu  (
       QWidget      * widget ,
       QPointF        pos    ,
       ControlPoint & From   ,
       ControlPoint & To     )
{
  AllowPickItemMenu                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  MenuManager mm ( widget )                                                  ;
  QAction   * aa                                                             ;
  QMenu     * ma                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  ma  = mm . addMenu   ( tr("Adjustments")    )                              ;
  AdjustMenu           ( mm , ma              )                              ;
  ////////////////////////////////////////////////////////////////////////////
  mm . addSeparator    (                      )                              ;
  mm . add             ( 901 , tr("Close")    )                              ;
  ////////////////////////////////////////////////////////////////////////////
  RunPickItemMenu                                                            ;
  nKickOut             ( IsNull ( aa ) , true )                              ;
  ////////////////////////////////////////////////////////////////////////////
  if ( AdjustProcessor ( mm , aa        ) ) return true                      ;
  if ( 0 != CallToMenu ( mm , aa , true ) ) return true                      ;
  ////////////////////////////////////////////////////////////////////////////
  switch ( mm [ aa ] )                                                       {
    case 901                                                                 :
      emit CloseObject ( node )                                              ;
    break                                                                    ;
  }                                                                          ;
  return true                                                                ;
}

#endif
