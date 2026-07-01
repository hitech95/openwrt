// SPDX-License-Identifier: GPL-2.0
/*
 * mt79xx-si3218x.c  --  MT79xx SI3218X ALSA SoC machine driver
 *
 * Copyright (c) 2021 MediaTek Inc.
 * Author: Vic Wu <vic.wu@mediatek.com>
 */

#include <linux/module.h>
#include <sound/soc.h>

#include "mt7986-afe-common.h"

static const struct snd_soc_dapm_widget mt7986_si3218x_widgets[] = {
	SND_SOC_DAPM_HP("Headphone", NULL),
	SND_SOC_DAPM_MIC("Microphone", NULL),
};

static const struct snd_soc_dapm_route mt7986_si3218x_routes[] = {
	{"Headphone", NULL, "VOUTP"},
	{"VINP", NULL, "Microphone"},
};

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
	DAILINK_COMP_ARRAY(COMP_CODEC(NULL, "slic-aif")),
	DAILINK_COMP_ARRAY(COMP_EMPTY()));

static struct snd_soc_dai_link mt7986_si3218x_dai_links[] = {
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
		.dai_fmt = SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF |
			SND_SOC_DAIFMT_CBS_CFS |
			SND_SOC_DAIFMT_GATED,
		.dpcm_playback = 1,
		.dpcm_capture = 1,
		SND_SOC_DAILINK_REG(codec),
	},
};

static struct snd_soc_card mt7986_si3218x_card = {
	.name = "mt7986-si3218x",
	.owner = THIS_MODULE,
	.dai_link = mt7986_si3218x_dai_links,
	.num_links = ARRAY_SIZE(mt7986_si3218x_dai_links),
	.dapm_widgets = mt7986_si3218x_widgets,
	.num_dapm_widgets = ARRAY_SIZE(mt7986_si3218x_widgets),
	.dapm_routes = mt7986_si3218x_routes,
	.num_dapm_routes = ARRAY_SIZE(mt7986_si3218x_routes),
};

static int mt7986_si3218x_machine_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card = &mt7986_si3218x_card;
	struct snd_soc_dai_link *dai_link;
	struct device_node *platform_dai_node, *codec_dai_node;
	int ret, i;

	card->dev = &pdev->dev;

	platform_dai_node = of_parse_phandle(pdev->dev.of_node, "mediatek,platform", 0);
	if (!platform_dai_node) {
		dev_err(&pdev->dev, "Property 'mediatek,platform' missing or invalid\n");
		return -EINVAL;
	}

	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->platforms->name)
			continue;
		dai_link->platforms->of_node = platform_dai_node;
	}

	codec_dai_node = of_parse_phandle(pdev->dev.of_node, "mediatek,codec", 0);
	if (!codec_dai_node) {
		dev_err(&pdev->dev, "Property 'mediatek,codec' missing or invalid\n");
		of_node_put(platform_dai_node);
		return -EINVAL;
	}

	for_each_card_prelinks(card, i, dai_link) {
		if (dai_link->codecs->name)
			continue;
		dai_link->codecs->of_node = codec_dai_node;
	}

	ret = devm_snd_soc_register_card(&pdev->dev, card);
	if (ret) {
		dev_err(&pdev->dev, "%s snd_soc_register_card fail: %d\n", __func__, ret);
		of_node_put(codec_dai_node);
		of_node_put(platform_dai_node);

		return ret;
	}

	return 0;
}

static const struct of_device_id mt7986_si3218x_machine_dt_match[] = {
	{.compatible = "mediatek,mt7986-si3218x-sound"},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mt7986_si3218x_machine_dt_match);

static struct platform_driver mt7986_si3218x_machine = {
	.driver = {
		.name = "mt7986-si3218x",
		.of_match_table = mt7986_si3218x_machine_dt_match,
	},
	.probe = mt7986_si3218x_machine_probe,
};

module_platform_driver(mt7986_si3218x_machine);

/* Module information */
MODULE_DESCRIPTION("MT7986 si3218x ALSA SoC machine driver");
MODULE_AUTHOR("Vic Wu <vic.wu@mediatek.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("mt7986 si3218x soc card");
