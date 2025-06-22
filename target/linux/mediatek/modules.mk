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

define KernelPackage/sound-soc-mt7986-si3218x
  TITLE:=Mediatek MT7986 sound card with SI3218x SLIC codec
  KCONFIG:=CONFIG_SND_SOC_MT7986_SI3218X
  FILES:=$(LINUX_DIR)/sound/soc/mediatek/mt7986/mt7986-si3218x.ko
  AUTOLOAD:=$(call AutoLoad,57,mt7986-si3218x)
  DEPENDS:=@TARGET_mediatek_filogic +kmod-sound-soc-slic-dummy +kmod-sound-soc-mt7986
  $(call AddDepends/sound)
endef

define KernelPackage/sound-soc-mt7986-si3218x/description
  Support for use the SiLab si3218x codec as a dummy sound card
endef

$(eval $(call KernelPackage,sound-soc-mt7986-si3218x))

define KernelPackage/sound-soc-slic-dummy
  TITLE:=Dummy SLIC codec support
  KCONFIG:=CONFIG_SND_SOC_SLIC_DUMMY
  FILES:=$(LINUX_DIR)/sound/soc/codecs/snd-soc-slic-dummy.ko
  DEPENDS:=+kmod-sound-soc-core
  AUTOLOAD:=$(call AutoProbe,snd-soc-slic-dummy)
  $(call AddDepends/sound)
endef

$(eval $(call KernelPackage,sound-soc-slic-dummy))
