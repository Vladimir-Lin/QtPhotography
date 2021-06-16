#include <qtphotography.h>

N::ImageSupports:: ImageSupports (QWidget * parent,Plan * p)
                 : TreeWidget    (          parent,       p)
{
  WidgetClass   ;
  Configure ( ) ;
}

N::ImageSupports::~ImageSupports (void)
{
}

QSize N::ImageSupports::sizeHint (void) const
{
  return QSize ( 640 , 480 ) ;
}

void N::ImageSupports::Configure (void)
{
  NewTreeWidgetItem            ( head                   ) ;
  head -> setText              ( 0    , tr("Format")    ) ;
  head -> setText              ( 1    , tr("Read"  )    ) ;
  head -> setText              ( 2    , tr("Write" )    ) ;
  head -> setText              ( 3    , ""              ) ;
  setAlignment                 ( 1    , Qt::AlignCenter ) ;
  setAlignment                 ( 2    , Qt::AlignCenter ) ;
  setAllAlignments             ( head , Qt::AlignCenter ) ;
  setWindowTitle               ( tr("Image formats")    ) ;
  setDragDropMode              ( NoDragDrop             ) ;
  setRootIsDecorated           ( false                  ) ;
  setAlternatingRowColors      ( true                   ) ;
  setSelectionMode             ( SingleSelection        ) ;
  setColumnCount               ( 4                      ) ;
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded  ) ;
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded  ) ;
  setAllAlignments             ( head , Qt::AlignCenter ) ;
  setFont                      ( head , Fonts::ListView ) ;
  setHeaderItem                ( head                   ) ;
  plan -> setFont              ( this                   ) ;
  List                         (                        ) ;
}

void N::ImageSupports::List (void)
{
  QList<QByteArray> IR = QImageReader::supportedImageFormats()  ;
  QList<QByteArray> WR = QImageWriter::supportedImageFormats()  ;
  QStringList       IS                                          ;
  QStringList       WS                                          ;
  QStringList       AS                                          ;
  clear ( )                                                     ;
  for (int i=0;i<IR.count();i++) IS << QString::fromUtf8(IR[i]) ;
  for (int i=0;i<WR.count();i++) WS << QString::fromUtf8(WR[i]) ;
  AS = IS                                                       ;
  for (int i=0;i<WS.count();i++) if (!AS.contains(WS[i]))       {
    AS << WS[i]                                                 ;
  }                                                             ;
  for (int i=0;i<AS.count();i++)                                {
    bool reading = IS.contains(AS[i])                           ;
    bool writing = WS.contains(AS[i])                           ;
    NewTreeWidgetItem ( IT )                                    ;
    IT -> setText ( 0 , AS[i]                          )        ;
    IT -> setText ( 1 , reading ? tr("Yes") : tr("No") )        ;
    IT -> setText ( 2 , writing ? tr("Yes") : tr("No") )        ;
    setAlignments ( IT                                 )        ;
    addTopLevelItem   ( IT )                                    ;
  }                                                             ;
}
