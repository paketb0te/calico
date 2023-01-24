// Project Calico BPF dataplane programs.
// Copyright (c) 2020-2022 Tigera, Inc. All rights reserved.
// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later

#include "ut.h"
#include "bpf.h"
#include "nat.h"

const volatile struct cali_tc_globals __globals;

static CALI_BPF_INLINE int calico_unittest_entry (struct __sk_buff *skb)
{
	struct cali_tc_ctx _ctx = {
		.counters = counters_get(skb->ifindex),
		.globals = &__globals,
		.skb = skb,
		.ipheader_len = IP_SIZE,
	};
	struct cali_tc_ctx *ctx = &_ctx;
	if (!ctx->counters) {
		CALI_DEBUG("Counters map lookup failed: DROP\n");
		return TC_ACT_SHOT;
	}
	return icmp_v4_ttl_exceeded(ctx);
}
