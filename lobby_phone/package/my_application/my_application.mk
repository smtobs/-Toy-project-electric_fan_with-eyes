MY_APPLICATION_VERSION = 1.0.0
MY_APPLICATION_SITE = $(TOPDIR)/package/my_application
MY_APPLICATION_SITE_METHOD = local
MY_APPLICATION_DEPENDENCIES = openssl spdlog opencv3 paho-mqtt-cpp yaml-cpp 

define MY_APPLICATION_BUILD_CMDS
   $(MAKE) -C $(@D) \
	   CXX=$(TARGET_CXX) \
	   CXXFLAGS="$(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) $(MY_CORE_CFLAGS)" \
	   LDFLAGS="$(TARGET_LDFLAGS) $(MY_CORE_LDFLAGS)" \
	   $(MY_CORE_MAKE_OPTS) \
	   all
endef

define MY_APPLICATION_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 755 $(@D)/core $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 644 $(MY_APPLICATION_SITE)/config/core_config.yaml $(TARGET_DIR)/usr/bin/core_config.yaml
endef

define MY_APPLICATION_INSTALL_INIT_SYSV
        $(INSTALL) -m 755 -D $(MY_APPLICATION_SITE)/S99core \
                $(TARGET_DIR)/etc/init.d/99core
endef

$(eval $(cmake-package))


