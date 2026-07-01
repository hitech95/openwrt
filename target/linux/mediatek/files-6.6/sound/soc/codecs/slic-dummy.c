/* Copyright (c) 2022-2023, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/of.h>
#include <sound/soc.h>
#include <linux/debugfs.h>
#include <sound/pcm_params.h>

static int dummy_component_probe(struct snd_soc_component *component)
{
	dev_info(component->dev, "%s\n", __func__);
	return 0;
}

static void dummy_component_remove(struct snd_soc_component *component)
{
	dev_info(component->dev, "%s\n", __func__);
}

static const struct snd_soc_dapm_widget dummy_component_dapm_widgets[] = {
	SND_SOC_DAPM_INPUT("VINP"),
	SND_SOC_DAPM_OUTPUT("VOUTP"),
};

static const struct snd_soc_dapm_route dummy_component_dapm_routes[] = {
	{ "VOUTP", NULL, "Playback"},
	{ "Capture", NULL, "VINP"},
};

static const struct snd_soc_component_driver dummy_component_driver = {
	.probe = dummy_component_probe,
	.remove = dummy_component_remove,

	.dapm_widgets = dummy_component_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(dummy_component_dapm_widgets),
	.dapm_routes = dummy_component_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(dummy_component_dapm_routes),

	.idle_bias_on = false,
};

static int dummy_component_aif_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *hw_params, struct snd_soc_dai *dai)
{
	int word_len = params_physical_width(hw_params);
	int aud_bit = params_width(hw_params);

	dev_info(dai->dev, "format: 0x%08x\n", params_format(hw_params));
	dev_info(dai->dev, "rate: 0x%08x\n", params_rate(hw_params));
	dev_info(dai->dev, "word_len: %d, aud_bit: %d\n", word_len, aud_bit);
	if (word_len != 16) {
		dev_err(dai->dev, "not supported word length\n");
		return -ENOTSUPP;
	}

	dev_info(dai->dev, "%s: --\n", __func__);
	return 0;
}

static const struct snd_soc_dai_ops dummy_component_aif_ops = {
	.hw_params = dummy_component_aif_hw_params,
};

static struct snd_soc_dai_driver dummy_codec_dai = {
	.name = "wm8960-hifi",
	.playback = {
		.stream_name	= "Playback",
		.channels_min	= 1,
		.channels_max	= 2,
		.rates		= SNDRV_PCM_RATE_16000,
		.formats	= SNDRV_PCM_FMTBIT_S16_LE,
	},
	.capture = {
		.stream_name	= "Capture",
		.channels_min	= 1,
		.channels_max	= 2,
		.rates 		= SNDRV_PCM_RATE_16000,
		.formats 	= SNDRV_PCM_FMTBIT_S16_LE,
	},
	/* dai properties */
	.symmetric_rate = 1,
	.symmetric_channels = 1,
	.symmetric_sample_bits = 1,
	/* dai operations */
	.ops = &dummy_component_aif_ops,
};

static int dummy_codec_probe(struct platform_device *pdev)
{
	int ret;
	dev_info(&pdev->dev, "%s: begin probe node: %s\n", __func__, pdev->name);

	ret = snd_soc_register_component(&pdev->dev, &dummy_component_driver,
				      &dummy_codec_dai, 1);
	if (ret) {
		dev_err(&pdev->dev, "Failed to snd_soc_register_component: %d\n", ret);
		return ret;
	}

	return ret;
}

static int dummy_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_component(&pdev->dev);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id dummy_codec_dt_match[] = {
	{.compatible = "dummy-codec",},
	{}
};
MODULE_DEVICE_TABLE(of, dummy_codec_dt_match);
#endif

static struct platform_driver dummy_codec = {
	.driver = {
	   .name = "dummy-codec",
	   .of_match_table = dummy_codec_dt_match,
	},
	.probe = dummy_codec_probe,
	.remove = dummy_codec_remove
};
module_platform_driver(dummy_codec);

/* Module information */
MODULE_DESCRIPTION("Dummy ASoC Codec");
MODULE_LICENSE("GPL");
