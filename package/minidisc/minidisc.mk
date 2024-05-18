################################################################################
#
# minidisc
#
################################################################################

MINIDISC_VERSION = 1.0
MINIDISC_SITE_METHOD = local
MINIDISC_SITE = $(BR2_EXTERNAL_MINIDISC_PATH)/package/minidisc/src
MINIDISC_INSTALL_STAGING = NO
MINIDISC_INSTALL_TARGET = YES
MINIDISC_DEPENDENCIES = libgpiod mpg123 gd

define MINIDISC_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/minidisc $(TARGET_DIR)/usr/bin
	$(INSTALL) -m 0755 -D $(BR2_EXTERNAL_MINIDISC_PATH)/package/minidisc/S42minidisc $(TARGET_DIR)/etc/init.d/S42minidisc
endef



$(eval $(cmake-package))
