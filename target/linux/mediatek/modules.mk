define KernelPackage/ata-ahci-mtk
  TITLE:=Mediatek AHCI Serial ATA support
  KCONFIG:=CONFIG_AHCI_MTK
  FILES:= \
	$(LINUX_DIR)/drivers/ata/ahci_mtk.ko \
	$(LINUX_DIR)/drivers/ata/libahci_platform.ko
  AUTOLOAD:=$(call AutoLoad,40,libahci libahci_platform ahci_mtk,1)
  $(call AddDepends/ata)
  DEPENDS+=@(TARGET_mediatek_mt7622||TARGET_mediatek_mt7623)
endef

define KernelPackage/ata-ahci-mtk/description
 Mediatek AHCI Serial ATA host controllers
endef

$(eval $(call KernelPackage,ata-ahci-mtk))

define KernelPackage/btmtkuart
  SUBMENU:=Other modules
  TITLE:=MediaTek HCI UART driver
  DEPENDS:=@TARGET_mediatek_mt7622 +kmod-bluetooth +mt7622bt-firmware
  KCONFIG:=CONFIG_BT_MTKUART
  FILES:= \
	$(LINUX_DIR)/drivers/bluetooth/btmtkuart.ko
  AUTOLOAD:=$(call AutoProbe,btmtkuart)
endef

$(eval $(call KernelPackage,btmtkuart))

define KernelPackage/iio-mt6577-auxadc
  TITLE:=Mediatek AUXADC driver
  DEPENDS:=@(TARGET_mediatek_mt7622||TARGET_mediatek_filogic)
  KCONFIG:=CONFIG_MEDIATEK_MT6577_AUXADC
  FILES:= \
	$(LINUX_DIR)/drivers/iio/adc/mt6577_auxadc.ko
  AUTOLOAD:=$(call AutoProbe,mt6577_auxadc)
  $(call AddDepends/iio)
endef
$(eval $(call KernelPackage,iio-mt6577-auxadc))

define KernelPackage/sound-soc-mt79xx
  TITLE:=MT79xx SoC sound support
  DEPENDS:=@TARGET_mediatek_filogic +kmod-sound-soc-core
  KCONFIG:=\
	CONFIG_SND_SOC_MEDIATEK \
	CONFIG_SND_SOC_MT79XX
  FILES:= \
	$(LINUX_DIR)/sound/soc/mediatek/common/snd-soc-mtk-common.ko \
	$(LINUX_DIR)/sound/soc/mediatek/mt79xx/snd-soc-mt79xx-afe.ko
  AUTOLOAD:=$(call AutoLoad,57,snd-soc-mtk-common snd-soc-mt79xx-afe)
  $(call AddDepends/sound)
endef

define KernelPackage/sound-soc-mt79xx/description
 Support for MT79xx Platform sound
endef

$(eval $(call KernelPackage,sound-soc-mt79xx))

define KernelPackage/sound-soc-mt79xx-wm8960
  TITLE:=MT79xx WM8960 sound card
  DEPENDS:=@TARGET_mediatek_filogic +kmod-regmap-i2c +kmod-sound-soc-mt79xx
  KCONFIG:=\
	CONFIG_SND_SOC_MT79XX_WM8960
  FILES:= \
	$(LINUX_DIR)/sound/soc/mediatek/mt79xx/mt79xx-wm8960.ko \
	$(LINUX_DIR)/sound/soc/codecs/snd-soc-wm8960.ko
  AUTOLOAD:=$(call AutoLoad,57,regmap-i2c snd-soc-mt79xx-afe snd-soc-wm8960  mt79xx-wm8960)
  $(call AddDepends/sound,+kmod-regmap-i2c)
endef

define KernelPackage/sound-soc-mt79xx-wm8960/description
 Support for MT79xx WM8960 sound card
endef

$(eval $(call KernelPackage,sound-soc-mt79xx-wm8960))

define KernelPackage/sound-soc-mt79xx-si3218x
  TITLE:=MT79xx SI3218x SLIC
  DEPENDS:=@TARGET_mediatek +kmod-sound-soc-mt79xx
  KCONFIG:=\
	CONFIG_SND_SOC_MT79XX_SI3218X
  FILES:= \
	$(LINUX_DIR)/sound/soc/mediatek/mt79xx/mt79xx-si3218x.ko \
	$(LINUX_DIR)/sound/soc/codecs/snd-soc-slic-dummy.ko
  AUTOLOAD:=$(call AutoLoad,57, snd-soc-mt79xx-afe snd-soc-slic-dummy mt79xx-si3218x)
  $(call AddDepends/sound)
endef

define KernelPackage/sound-soc-mt79xx-si3218x/description
 Support for MT79xx and dummy si3218x sound card
endef

$(eval $(call KernelPackage,sound-soc-mt79xx-si3218x))
