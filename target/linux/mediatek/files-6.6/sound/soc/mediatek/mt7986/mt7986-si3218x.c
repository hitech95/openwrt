// SPDX-License-Identifier: GPL-2.0
/*
 * mt79xx-si3218x.c  --  MT79xx WM8960 ALSA SoC machine driver
 *
 * Copyright (c) 2021 MediaTek Inc.
 * Author: Vic Wu <vic.wu@mediatek.com>
 */

#include <linux/module.h>
#include <linux/of.h>
#include <sound/soc.h>

#include "mt7986-reg.h"
#include "mt7986-afe-common.h"
#include "../common/mtk-afe-platform-driver.h"

#define MTK_DAI_ETDM_FORMAT_I2S   0
#define MTK_DAI_ETDM_FORMAT_DSPA  4
#define MTK_DAI_ETDM_FORMAT_DSPB  5

enum {
	MTK_AFE_RATE_8K = 0,
	MTK_AFE_RATE_11K = 1,
	MTK_AFE_RATE_12K = 2,
	MTK_AFE_RATE_16K = 4,
	MTK_AFE_RATE_22K = 5,
	MTK_AFE_RATE_24K = 6,
	MTK_AFE_RATE_32K = 8,
	MTK_AFE_RATE_44K = 9,
	MTK_AFE_RATE_48K = 10,
	MTK_AFE_RATE_88K = 13,
	MTK_AFE_RATE_96K = 14,
	MTK_AFE_RATE_176K = 17,
	MTK_AFE_RATE_192K = 18,
};

enum {
	MTK_ETDM_RATE_8K = 0,
	MTK_ETDM_RATE_12K = 1,
	MTK_ETDM_RATE_16K = 2,
	MTK_ETDM_RATE_24K = 3,
	MTK_ETDM_RATE_32K = 4,
	MTK_ETDM_RATE_48K = 5,
	MTK_ETDM_RATE_96K = 7,
	MTK_ETDM_RATE_192K = 9,
	MTK_ETDM_RATE_11K = 16,
	MTK_ETDM_RATE_22K = 17,
	MTK_ETDM_RATE_44K = 18,
	MTK_ETDM_RATE_88K = 19,
	MTK_ETDM_RATE_176K = 20,
};

enum {
	MTK_ETDM_OUTSEL_IN5 = 2,
	MTK_ETDM_OUTSEL_OUT5 = 10,
};

struct mt79xx_si3218x_priv {
	struct device_node *platform_dai_node;
	struct device_node *codec_dai_node;
};

static int mt79xx_si3218x_init(struct snd_soc_pcm_runtime *rtd)
{
	struct snd_soc_component *component =
		snd_soc_rtdcom_lookup(rtd, AFE_PCM_NAME);
	struct mtk_base_afe *afe = snd_soc_component_get_drvdata(component);

	/* set ETDM_IN5_CON0 */
	// PCM-A / I2S format
	regmap_update_bits(afe->regmap, ETDM_IN5_CON0, ETDM_FMT_MASK,
			   FIELD_PREP(ETDM_FMT_MASK, MTK_DAI_ETDM_FORMAT_I2S));
	// 16 bit for WORDs
	regmap_update_bits(afe->regmap, ETDM_IN5_CON0, ETDM_BIT_LEN_MASK,
			   FIELD_PREP(ETDM_BIT_LEN_MASK, 16 - 1));
	regmap_update_bits(afe->regmap, ETDM_IN5_CON0, ETDM_WRD_LEN_MASK,
			   FIELD_PREP(ETDM_WRD_LEN_MASK, 16 - 1));
	// 4 channels
	regmap_update_bits(afe->regmap, ETDM_IN5_CON0, ETDM_CH_NUM_MASK,
			   FIELD_PREP(ETDM_CH_NUM_MASK, 4 - 1));

	/* set ETDM_IN5_CON3 */
	regmap_update_bits(afe->regmap, ETDM_IN5_CON3, IN_SEL_FS_MASK,
			   IN_SEL_FS(MTK_ETDM_RATE_16K));

	/* set ETDM_IN5_CON4 */
	regmap_update_bits(afe->regmap, ETDM_IN5_CON4, IN_CLK_INV_MASK,
			   IN_CLK_INV);
	regmap_update_bits(afe->regmap, ETDM_IN5_CON4, IN_RELATCH_MASK,
			   IN_RELATCH(MTK_AFE_RATE_16K));

	/* set ETDM_OUT5_CON0 */
	// PCM-A / I2S format
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON0, ETDM_FMT_MASK,
			   FIELD_PREP(ETDM_FMT_MASK, MTK_DAI_ETDM_FORMAT_I2S));
	// 16 bit for WORDs
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON0, ETDM_BIT_LEN_MASK,
			   FIELD_PREP(ETDM_BIT_LEN_MASK, 16 - 1));
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON0, ETDM_WRD_LEN_MASK,
			   FIELD_PREP(ETDM_WRD_LEN_MASK, 16 - 1));
	// 4 channels
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON0, ETDM_CH_NUM_MASK,
			   FIELD_PREP(ETDM_CH_NUM_MASK, 4 - 1));

	/* set ETDM_OUT5_CON4 */
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON4, OUT_SEL_FS_MASK,
			   OUT_SEL_FS(MTK_ETDM_RATE_16K));
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON4, OUT_RELATCH_MASK,
			   OUT_RELATCH(MTK_AFE_RATE_16K));

	/* set ETDM_OUT5_CON5 */
	regmap_update_bits(afe->regmap, ETDM_OUT5_CON5, OUT_CLK_INV_MASK,
			   OUT_CLK_INV);

	/* set external loopback */
	regmap_update_bits(afe->regmap, ETDM_4_7_COWORK_CON0, OUT_SEL_MASK,
			   OUT_SEL(MTK_ETDM_OUTSEL_IN5));

	return 0;
}

SND_SOC_DAILINK_DEFS(playback,
	DAILINK_COMP_ARRAY(COMP_CPU("DL1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(capture,
	DAILINK_COMP_ARRAY(COMP_CPU("UL1")),
	DAILINK_COMP_ARRAY(COMP_DUMMY()),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

SND_SOC_DAILINK_DEFS(codec,
	DAILINK_COMP_ARRAY(COMP_CPU("ETDM")),
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "proslic_spi-aif")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link mt79xx_si3218x_dai_links[] = {
	/* FE */
	{
		.name = "si3218x-playback",
		.stream_name = "si3218x-playback",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.dynamic = 1,
		.dpcm_playback = 1,
		SND_SOC_DAILINK_REG(playback),
	},
	{
		.name = "si3218x-capture",
		.stream_name = "si3218x-capture",
		.trigger = {SND_SOC_DPCM_TRIGGER_POST,
			    SND_SOC_DPCM_TRIGGER_POST},
		.dynamic = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(capture),
	},
	/* BE */
	{
		.name = "si3218x-codec",
		.no_pcm = 1,
		.dai_fmt = SND_SOC_DAIFMT_DSP_A |
			SND_SOC_DAIFMT_IB_NF |
			SND_SOC_DAIFMT_CBS_CFS,
		.init = mt79xx_si3218x_init,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(codec),
	},
};

static struct snd_soc_card mt79xx_si3218x_card = {
	.name = "mt79xx-si3218x",
	.owner = THIS_MODULE,
	.dai_link = mt79xx_si3218x_dai_links,
	.num_links = ARRAY_SIZE(mt79xx_si3218x_dai_links),
};

static int mt79xx_si3218x_machine_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &mt79xx_si3218x_card;
	struct snd_soc_dai_link *dai_link;
	struct mt79xx_si3218x_priv *priv;
	int ret, i;

	card->dev = &pdev->dev;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->platform_dai_node = of_parse_phandle(pdev->dev.of_node, "mediatek,platform", 0);
	if (!priv->platform_dai_node) {
		dev_err(&pdev->dev, "Property 'platform' missing or invalid\n");
		return -EINVAL;
	}

	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->platforms->name)
			continue;
		dai_link->platforms->of_node = priv->platform_dai_node;
	}

	priv->codec_dai_node = of_parse_phandle(pdev->dev.of_node, "mediatek,codec", 0);
	if (!priv->codec_dai_node) {
		dev_err(&pdev->dev, "Property 'mediatek,codec' missing or invalid\n");
		of_node_put(priv->platform_dai_node);
		return -EINVAL;
	}

	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->codecs->name)
			continue;
		dai_link->codecs->of_node = priv->codec_dai_node;
	}

	snd_soc_card_set_drvdata(card, priv);

	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret) {
		dev_err_probe(&pdev->dev, ret, "%s snd_soc_register_card fail\n", __func__);
		goto err_of_node_put;
	}

err_of_node_put:
	of_node_put(priv->platform_dai_node);
	of_node_put(priv->codec_dai_node);
	return ret;
}

static int mt79xx_si3218x_machine_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	struct mt79xx_si3218x_priv *priv = snd_soc_card_get_drvdata(card);

	of_node_put(priv->codec_dai_node);
	of_node_put(priv->platform_dai_node);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id mt79xx_si3218x_machine_dt_match[] = {
	{.compatible = "mediatek,mt7986-si3218x-machine",},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mt79xx_si3218x_machine_dt_match);
#endif

static struct platform_driver mt79xx_si3218x_machine = {
	.driver = {
		.name = "mt7986-si3218x",
#ifdef CONFIG_OF
		.of_match_table = mt79xx_si3218x_machine_dt_match,
#endif
	},
	.probe = mt79xx_si3218x_machine_probe,
	.remove = mt79xx_si3218x_machine_remove,
};

module_platform_driver(mt79xx_si3218x_machine);

/* Module information */
MODULE_DESCRIPTION("MT79xx SI3218x ALSA SoC machine driver");
MODULE_AUTHOR("Vic Wu <vic.wu@mediatek.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("mt79xx si3218x soc card");
