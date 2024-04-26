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
MINIDISC_DEPENDENCIES = libgpiod mpg123

$(eval $(cmake-package))
