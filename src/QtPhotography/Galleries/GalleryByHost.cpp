#include <qtphotography.h>
#include "ui_GalleryByHost.h"

N::GalleryByHost:: GalleryByHost ( QWidget * parent , Plan * p )
                 : Dialog        (           parent ,        p )
                 , ui            ( new Ui::GalleryByHost       )
{
  WidgetClass              ;
  ui   -> setupUi ( this ) ;
  plan -> setFont ( this ) ;
}

N::GalleryByHost::~GalleryByHost(void)
{
  delete ui ;
}

SUID N::GalleryByHost::TargetUuid(void)
{
  return N::GetUuid(ui->Gallery) ;
}

bool N::GalleryByHost::Prepare(void)
{
  plan -> setFont ( this )                ;
  N::SqlConnection SC ( plan->sql )       ;
  N::GroupItems    GI ( plan      )       ;
  ui->Gallery->clear()                    ;
  if (SC.open("GalleryByHost","Prepare")) {
    UUIDs   Uuids                         ;
    SUID    uuid                          ;
    Uuids = GI.Groups                     (
              SC                          ,
              N::Types::Gallery           ,
              SC.OrderByAsc("position") ) ;
    foreach (uuid,Uuids)                  {
      QString name                        ;
      name = SC.getName                   (
               PlanTable(Names)           ,
               "uuid"                     ,
               vLanguageId                ,
               uuid                     ) ;
      ui->Gallery->addItem(name,uuid)     ;
    }                                     ;
    SC.close()                            ;
  }                                       ;
  SC.remove()                             ;
  return true                             ;
}

bool N::GalleryByHost::Categorize(void)
{
  QString h = ui->Host->toPlainText()                   ;
  if (h.length()<=0) return false                       ;
  QStringList Hs = h.split('\n')                        ;
  if (Hs.count()<=0) return false                       ;
  ///////////////////////////////////////////////////////
  QList<QUrl> URLs                                      ;
  for (int i=0;i<Hs.count();i++)                        {
    QString v = Hs[i]                                   ;
    v = v.replace("\r","")                              ;
    v = v.replace("\n","")                              ;
    if (!v.contains("http://"))                         {
      v.prepend("http://")                              ;
    }                                                   ;
    QUrl url(v)                                         ;
    if (url.isValid())                                  {
      URLs << url                                       ;
    }                                                   ;
  }                                                     ;
  if (URLs.count()<=0) return false                     ;
  ///////////////////////////////////////////////////////
  N::ScopedProgress X(plan,tr("%v/%m URLs"),1)          ;
  SUID guid = TargetUuid()                              ;
  SUID site = 0                                         ;
  nKickOut             ( guid <= 0 , false   )          ;
  N::SqlConnection  SC ( plan->sql           )          ;
  N::GroupItems     GI ( plan                )          ;
  N::NetworkManager NM ( plan                )          ;
  if (SC.open("GalleryByHost","Categorize"))            {
    QString Q                                           ;
    QUrl    url                                         ;
    QString host                                        ;
    X . show     (                  )                   ;
    X . setRange ( 0 , URLs.count() )                   ;
    for (int i=0;i<URLs.count();i++)                    {
      ++X                                               ;
      url  = URLs[i]                                    ;
      host = url.host()                                 ;
      site = NM.UrlUuid(SC,url)                         ;
      if (site>0)                                       {
        UUIDs Puids                                     ;
        UUIDs Uuids                                     ;
        plan->showMessage(tr("Searching %1").arg(host)) ;
        DoProcessEvents                                 ;
        Puids = SC.Uuids                                (
                  PlanTable(SitePages)                  ,
                  "page"                                ,
                  QString("where site = %1 %2"          )
                  .arg(site                             )
                  .arg(SC.OrderByAsc("id")              )
                )                                       ;
        if (Puids.count()>0)                            {
          Uuids = GI.RelateUuids                        (
                    SC                                  ,
                    Puids                               ,
                    N :: Types  :: Picture              ,
                    N :: Types  :: URL                  ,
                    N :: Groups :: Subordination        ,
                    true,tr("%v/%m")                  ) ;
        }                                               ;
        if (Uuids.count()>0)                            {
          plan->showMessage(tr("Join assigned group"))  ;
          DoProcessEvents                               ;
          GI.Join                                       (
            SC                                          ,
            guid                                        ,
            N :: Types  :: Gallery                      ,
            N :: Types  :: Picture                      ,
            N :: Groups :: Subordination                ,
            0,Uuids                                   ) ;
        }                                               ;
      }                                                 ;
    }                                                   ;
    SC . close()                                        ;
  }                                                     ;
  SC.remove()                                           ;
  Alert ( Done )                                        ;
  return true                                           ;
}
