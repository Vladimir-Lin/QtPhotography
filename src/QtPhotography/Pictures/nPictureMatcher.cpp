#include <qtphotography.h>

typedef struct         {
  QString Message      ;
  UUIDs   Uuids        ;
} PictureMatcherPacket ;

#define plan PicturePlan

N::PictureMatcher:: PictureMatcher ( QObject * parent , Plan * p )
                  : QObject        (           parent            )
                  , Thread         (                             )
                  , PictureManager (                           p )
                  , GroupItems     (                           p )
{
  WidgetClass                                             ;
  nConnect ( this , SIGNAL ( sendMessage  ( QString ) )   ,
             this , SLOT   ( typeMessage  ( QString ) ) ) ;
  nConnect ( this , SIGNAL ( OnBusy       (         ) )   ,
             this , SLOT   ( AtBusy       (         ) ) ) ;
  nConnect ( this , SIGNAL ( GoRelax      (         ) )   ,
             this , SLOT   ( OnRelax      (         ) ) ) ;
  nConnect ( this , SIGNAL ( PostPictures (         ) )   ,
             this , SLOT   ( SendPictures (         ) ) ) ;
}

N::PictureMatcher::~PictureMatcher (void)
{
}

void N::PictureMatcher::ThreadEvent(void)
{
  plan -> processEvents ( ) ;
}

void N::PictureMatcher::AtBusy(void)
{
  plan->StartBusy ( ) ;
}

void N::PictureMatcher::OnRelax(void)
{
  plan->StopBusy ( ) ;
}

UUIDs N::PictureMatcher::toUuids(VarArgs & V)
{
  UUIDs U                   ;
  if (V.count()<2) return U ;
  int   t = V[0].toInt()    ;
  int   p = 1               ;
  for (int i=0;i<t;i++,p++) {
    U << V[p].toULongLong() ;
  }                         ;
  return U                  ;
}

void N::PictureMatcher::Search(SUID ID,UUIDs & Uuids)
{
  VarArgs V                 ;
  int     T = Uuids.count() ;
  V << T                    ;
  toVariants ( Uuids , V  ) ;
  start      ( ID , V     ) ;
}

void N::PictureMatcher::showMessage(QString message)
{
  emit sendMessage ( message ) ;
}

void N::PictureMatcher::typeMessage(QString message)
{
  plan -> showMessage ( message ) ;
}

void N::PictureMatcher::SendPictures (void)
{
  QMutexLocker L ( &SendMutex )                                    ;
  QList<PictureMatcherPacket *> PMPs                               ;
  while (Queues.count()>0)                                         {
    QString M                                                      ;
    UUIDs   U                                                      ;
    PictureMatcherPacket * PMP = (PictureMatcherPacket *)Queues[0] ;
    Queues . takeAt ( 0 )                                          ;
    PMPs << PMP                                                    ;
    M = PMP -> Message                                             ;
    U = PMP -> Uuids                                               ;
    emit ViewPictures ( M , U )                                    ;
  }                                                                ;
  Time :: skip ( 500 )                                             ;
  for (int i=0;i<PMPs.count();i++) delete PMPs[i]                  ;
  PMPs . clear ( )                                                 ;
}

void N::PictureMatcher::run(int Type,ThreadData * data)
{
  VarArgs V = data->Arguments ;
  UUIDs   U                   ;
  switch ( Type )             {
    case 1                    :
      U = toUuids  ( V )      ;
      SizeMatch    ( U )      ;
    break                     ;
    case 2                    :
      U = toUuids  ( V )      ;
      ColorsMatch  ( U )      ;
    break                     ;
    case 3                    :
      U = toUuids  ( V )      ;
      PaletteMatch ( U )      ;
    break                     ;
    case 4                    :
      U = toUuids  ( V )      ;
      ExactMatch   ( U )      ;
    break                     ;
  }                           ;
}

void N::PictureMatcher::SizeMatch(UUIDs & U)
{
  QString M                                                                   ;
  UUIDs   R                                                                   ;
  emit OnBusy         (                  )                                    ;
  SqlConnection SC    ( plan->sql        )                                    ;
  if (SC.open("PictureMatcher","SizeMatch"))                                  {
    QList<QSize> SIZEs                                                        ;
    M = tr("Search database for exact size of %1 pictures.").arg(U.count())   ;
    showMessage       ( M                )                                    ;
    if ( ObtainSizes  ( SC , U , SIZEs   )                                  ) {
      ObtainBySizes   ( SC , R , SIZEs   )                                    ;
    }                                                                         ;
  } else                                                                      {
    M = tr("Failure to access database.")                                     ;
    showMessage       ( M                )                                    ;
  }                                                                           ;
  emit GoRelax        (                  )                                    ;
  if (R.count()>0)                                                            {
    M = tr("%1 pictures match with your selected pictures.").arg(R.count())   ;
    showMessage       ( M                )                                    ;
    Alert             ( Done             )                                    ;
    PictureMatcherPacket * PMP = new PictureMatcherPacket                     ;
    PMP -> Message = M                                                        ;
    PMP -> Uuids   = R                                                        ;
    Queues << PMP                                                             ;
    emit PostPictures (                  )                                    ;
  } else                                                                      {
    M = tr("There are no picture matching with your selected pictures size.") ;
    showMessage       ( M                )                                    ;
    Alert             ( Error            )                                    ;
  }                                                                           ;
}

void N::PictureMatcher::ColorsMatch(UUIDs & U)
{
  QString M                                                                     ;
  UUIDs   R                                                                     ;
  emit OnBusy         (                  )                                      ;
  SqlConnection SC    ( plan->sql        )                                      ;
  if (SC.open("PictureMatcher","ColorsMatch"))                                  {
    CUIDs COLORs                                                                ;
    M = tr("Search database for colors used of %1 pictures.").arg(U.count())    ;
    showMessage       ( M                )                                      ;
    if ( ObtainColors ( SC , U , COLORs  )                                    ) {
      ObtainByColors  ( SC , R , COLORs  )                                      ;
    }                                                                           ;
  } else                                                                        {
    M = tr("Failure to access database.")                                       ;
    showMessage       ( M                )                                      ;
  }                                                                             ;
  emit GoRelax        (                  )                                      ;
  if (R.count()>0)                                                              {
    M = tr("%1 pictures match with your selected pictures.").arg(R.count())     ;
    showMessage       ( M                )                                      ;
    Alert             ( Done             )                                      ;
    PictureMatcherPacket * PMP = new PictureMatcherPacket                       ;
    PMP -> Message = M                                                          ;
    PMP -> Uuids   = R                                                          ;
    Queues << PMP                                                               ;
    emit PostPictures (                  )                                      ;
  } else                                                                        {
    M = tr("There are no picture matching with your selected pictures colors.") ;
    showMessage       ( M                )                                      ;
    Alert             ( Error            )                                      ;
  }                                                                             ;
}

void N::PictureMatcher::PaletteMatch(UUIDs & U)
{
  QString M                                                                      ;
  UUIDs   R                                                                      ;
  emit OnBusy         (                  )                                       ;
  SqlConnection SC    ( plan->sql        )                                       ;
  if (SC.open("PictureMatcher","PaletteMatch"))                                  {
    UUIDs PALs                                                                   ;
    M = tr("Search database for exact palette of %1 pictures.").arg(U.count())   ;
    showMessage       ( M                )                                       ;
    if ( ObtainPALs   ( SC , U , PALs    )                                     ) {
      ObtainByPALs    ( SC , R , PALs    )                                       ;
    }                                                                            ;
  } else                                                                         {
    M = tr("Failure to access database.")                                        ;
    showMessage       ( M                )                                       ;
  }                                                                              ;
  emit GoRelax        (                  )                                       ;
  if (R.count()>0)                                                               {
    M = tr("%1 pictures match with your selected pictures.").arg(R.count())      ;
    showMessage       ( M                )                                       ;
    Alert             ( Done             )                                       ;
    PictureMatcherPacket * PMP = new PictureMatcherPacket                        ;
    PMP -> Message = M                                                           ;
    PMP -> Uuids   = R                                                           ;
    Queues << PMP                                                                ;
    emit PostPictures (                  )                                       ;
  } else                                                                         {
    M = tr("There are no picture matching with your selected pictures palette.") ;
    showMessage       ( M                )                                       ;
    Alert             ( Error            )                                       ;
  }                                                                              ;
}

void N::PictureMatcher::ExactMatch(UUIDs & U)
{
  QString M                                                                          ;
  UUIDs   R                                                                          ;
  emit OnBusy         (                  )                                           ;
  SqlConnection SC    ( plan->sql        )                                           ;
  if (SC.open("PictureMatcher","ExactMatch"))                                        {
    QList<QSize> SIZEs                                                               ;
    M = tr("Search database for exact contexts of %1 pictures.").arg(U.count())      ;
    showMessage ( M    )                                                             ;
    if ( ObtainSizes  ( SC , U , SIZEs   )                                         ) {
      ObtainBySizes   ( SC , R , SIZEs   )                                           ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      UUIDs Z = R                                                                    ;
      Z = Z - U                                                                      ;
      if ( Z . count ( ) <= 0 ) R . clear ( )                                        ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      CUIDs C                                                                        ;
      if ( ObtainColors ( SC , U , C ) )                                             {
        UUIDs P                                                                      ;
        ObtainByColors  ( SC , P , C   )                                             ;
        R = R & P                                                                    ;
      } else R . clear ( )                                                           ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      UUIDs Z = R                                                                    ;
      Z = Z - U                                                                      ;
      if ( Z . count ( ) <= 0 ) R . clear ( )                                        ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      UUIDs L                                                                        ;
      if ( ObtainPALs ( SC , U , L ) )                                               {
        UUIDs P                                                                      ;
        ObtainByPALs  ( SC , P , L   )                                               ;
        R = R & P                                                                    ;
      } else R . clear ( )                                                           ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      UUIDs Z = R                                                                    ;
      Z = Z - U                                                                      ;
      if ( Z . count ( ) <= 0 ) R . clear ( )                                        ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      ByteArrays H                                                                   ;
      if ( ObtainHistos ( SC , U , H ) )                                             {
        UUIDs P                                                                      ;
        ObtainByHistos  ( SC , P , H   )                                             ;
        R = R & P                                                                    ;
      } else R . clear ( )                                                           ;
    }                                                                                ;
    if ( R . count ( ) > 0 )                                                         {
      UUIDs Z = R                                                                    ;
      Z = Z - U                                                                      ;
      if ( Z . count ( ) <= 0 ) R . clear ( )                                        ;
    }                                                                                ;
  } else                                                                             {
    M = tr("Failure to access database.")                                            ;
    showMessage       ( M                )                                           ;
  }                                                                                  ;
  emit GoRelax        (                  )                                           ;
  if (R.count()>0)                                                                   {
    M = tr("%1 pictures exact match with your selected pictures.").arg(R.count())    ;
    showMessage       ( M                )                                           ;
    Alert             ( Done             )                                           ;
    PictureMatcherPacket * PMP = new PictureMatcherPacket                            ;
    PMP -> Message = M                                                               ;
    PMP -> Uuids   = R                                                               ;
    Queues << PMP                                                                    ;
    emit PostPictures (                  )                                           ;
  } else                                                                             {
    M = tr("There are no other picture exact matching with your selected pictures.") ;
    showMessage       ( M                )                                           ;
    Alert             ( Error            )                                           ;
  }                                                                                  ;
}
