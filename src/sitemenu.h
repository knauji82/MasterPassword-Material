#ifndef SITEMENU
#define SITEMENU

#include "site.h"

#include <QMenu>

class SiteMenu : public QMenu
{
  Q_OBJECT

private:
  Site const *site_;

public:
  SiteMenu(Site const *site, QWidget *parent)
      : QMenu(site->name(), parent), site_(site)
  {
    addAction(tr("Password"))->setData(MPSiteVariantPassword);
    addAction(tr("Login"))->setData(MPSiteVariantLogin);
    addAction(tr("Answer"))->setData(MPSiteVariantAnswer);
  }

  inline Site const * site() const
  {
    return site_;
  }
};

#endif // SITEMENU

