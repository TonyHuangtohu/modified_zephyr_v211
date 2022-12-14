/*
 * Copyright (c) 2021 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <soc.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/drivers/clock_control/stm32_clock_control.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(test);

static void test_sysclk_freq(void)
{
	uint32_t soc_sys_clk_freq;

	soc_sys_clk_freq = HAL_RCC_GetSysClockFreq();

	zassert_equal(CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC, soc_sys_clk_freq,
			"Expected sysclockfreq: %d. Actual sysclockfreq: %d",
			CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC, soc_sys_clk_freq);
}

static void test_sysclk_src(void)
{
	int sys_clk_src = __HAL_RCC_GET_SYSCLK_SOURCE();

#if STM32_SYSCLK_SRC_PLL
	zassert_equal(RCC_SYSCLKSOURCE_STATUS_PLLCLK, sys_clk_src,
			"Expected sysclk src: PLL. Actual sysclk src: %d",
			sys_clk_src);
#elif STM32_SYSCLK_SRC_HSE
	zassert_equal(RCC_SYSCLKSOURCE_STATUS_HSE, sys_clk_src,
			"Expected sysclk src: HSE. Actual sysclk src: %d",
			sys_clk_src);
#elif STM32_SYSCLK_SRC_HSI
	zassert_equal(RCC_SYSCLKSOURCE_STATUS_HSI, sys_clk_src,
			"Expected sysclk src: HSI. Actual sysclk src: %d",
			sys_clk_src);
#elif STM32_SYSCLK_SRC_CSI
	zassert_equal(RCC_SYSCLKSOURCE_STATUS_CSI, sys_clk_src,
			"Expected sysclk src: CSI. Actual sysclk src: %d",
			sys_clk_src);
#else
	/* Case not expected */
	zassert_true((STM32_SYSCLK_SRC_PLL ||
		      STM32_SYSCLK_SRC_HSE ||
		      STM32_SYSCLK_SRC_HSI ||
		      STM32_SYSCLK_SRC_CSI),
		      "Not expected. sys_clk_src: %d\n", sys_clk_src);
#endif

}

static void test_pll_src(void)
{
	uint32_t pll_src = __HAL_RCC_GET_PLL_OSCSOURCE();

#if STM32_PLL_SRC_HSE
	zassert_equal(RCC_PLLSOURCE_HSE, pll_src,
			"Expected PLL src: HSE (%d). Actual PLL src: %d",
			RCC_PLLSOURCE_HSE, pll_src);
#elif STM32_PLL_SRC_HSI
	zassert_equal(RCC_PLLSOURCE_HSI, pll_src,
			"Expected PLL src: HSI (%d). Actual PLL src: %d",
			RCC_PLLSOURCE_HSI, pll_src);
#elif STM32_PLL_SRC_CSI
	zassert_equal(RCC_PLLSOURCE_CSI, pll_src,
			"Expected PLL src: CSI (%d). Actual PLL src: %d",
			RCC_PLLSOURCE_CSI, pll_src);
#else
	zassert_equal(RCC_PLLSOURCE_NONE, pll_src,
			"Expected PLL src: none (%d). Actual PLL src: %d",
			RCC_PLLSOURCE_NONE, pll_src);
#endif

}

void test_main(void)
{
	ztest_test_suite(test_stm32_syclck_config,
		ztest_unit_test(test_sysclk_freq),
		ztest_unit_test(test_sysclk_src),
		ztest_unit_test(test_pll_src)
			 );
	ztest_run_test_suite(test_stm32_syclck_config);
}
