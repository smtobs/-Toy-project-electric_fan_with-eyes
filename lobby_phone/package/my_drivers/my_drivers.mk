MY_DRIVERS_VERSION = 1.0.0
MY_DRIVERS_SITE = $(TOPDIR)/package/my_drivers
MY_DRIVERS_SITE_METHOD = local
 
$(eval $(kernel-module))
$(eval $(generic-package))
