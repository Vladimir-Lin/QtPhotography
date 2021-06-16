#include <qtphotography.h>
#include "ui_GalleryOperator.h"

N::GalleryOperator:: GalleryOperator ( QWidget * parent , Plan * p )
                   : Dialog          (           parent ,        p )
                   , ui              ( new Ui::GalleryOperator     )
{
  WidgetClass                                                              ;
  ui   -> setupUi ( this )                                                 ;
  ui   -> Source -> lineEdit() -> setPlaceholderText(tr("Source gallery")) ;
  ui   -> Target -> lineEdit() -> setPlaceholderText(tr("Target gallery")) ;
  plan -> setFont ( this )                                                 ;
}

N::GalleryOperator::~GalleryOperator(void)
{
  delete ui ;
}

SUID N::GalleryOperator::TargetUuid(void)
{
  return N::GetUuid(ui->Target) ;
}

SUID N::GalleryOperator::SourceUuid(void)
{
  return N::GetUuid(ui->Source) ;
}

bool N::GalleryOperator::Prepare(void)
{
  plan -> setFont ( this )                ;
  N::SqlConnection SC ( plan->sql )       ;
  N::GroupItems    GI ( plan      )       ;
  ui->Source  ->clear()                   ;
  ui->Target  ->clear()                   ;
  ui->Operator->clear()                   ;
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
      ui->Source->addItem(name,uuid)      ;
      ui->Target->addItem(name,uuid)      ;
    }                                     ;
    SC.close()                            ;
  }                                       ;
  SC.remove()                             ;
  ui->Operator->addItem(tr("Join"  ))     ;
  ui->Operator->addItem(tr("Detach"))     ;
  return true                             ;
}

bool N::GalleryOperator::Categorize(void)
{
  SUID suid = SourceUuid()                     ;
  SUID tuid = TargetUuid()                     ;
  int  op   = ui->Operator->currentIndex()     ;
  if (suid<=0) return false                    ;
  if (tuid<=0) return false                    ;
  N::SqlConnection SC ( plan->sql )            ;
  N::GroupItems    GI ( plan      )            ;
  if (SC.open("GalleryOperator","Categorize")) {
    switch (op)                                {
      case 0                                   :
        GI.CopyTo                              (
          SC                                   ,
          suid                                 ,
          tuid                                 ,
          N :: Types  :: Gallery               ,
          N :: Types  :: Picture               ,
          N :: Groups :: Subordination         ,
          true,tr("%v/%m")                   ) ;
      break                                    ;
      case 1                                   :
        GI.Substract                           (
          SC                                   ,
          suid                                 ,
          tuid                                 ,
          N :: Types  :: Gallery               ,
          N :: Types  :: Picture               ,
          N :: Groups :: Subordination         ,
          true,tr("%v/%m")                   ) ;
      break                                    ;
    }                                          ;
    SC . close()                               ;
  }                                            ;
  SC.remove()                                  ;
  Alert ( Done )                               ;
  return true                                  ;
}
