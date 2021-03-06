/*
 * Copyright 2014 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __AMDGPU_IRQ_H__
#define __AMDGPU_IRQ_H__

#include "amdgpu_ih.h"

#define AMDGPU_MAX_IRQ_SRC_ID	0x100

struct amdgpu_device;
struct amdgpu_iv_entry;

enum amdgpu_interrupt_state {
	AMDGPU_IRQ_STATE_DISABLE,
	AMDGPU_IRQ_STATE_ENABLE,
};

struct amdgpu_irq_src {
	unsigned				num_types;
	atomic_t				*enabled_types;
	const struct amdgpu_irq_src_funcs	*funcs;
};

/* provided by interrupt generating IP blocks */
struct amdgpu_irq_src_funcs {
	int (*set)(struct amdgpu_device *adev, struct amdgpu_irq_src *source,
		   unsigned type, enum amdgpu_interrupt_state state);

	int (*process)(struct amdgpu_device *adev,
		       struct amdgpu_irq_src *source,
		       struct amdgpu_iv_entry *entry);
};

struct amdgpu_irq {
	bool				installed;
	spinlock_t			lock;
	/* interrupt sources */
	struct amdgpu_irq_src		*sources[AMDGPU_MAX_IRQ_SRC_ID];

	/* status, etc. */
	bool				msi_enabled; /* msi enabled */

	/* interrupt ring */
	struct amdgpu_ih_ring		ih;
	const struct amdgpu_ih_funcs	*ih_funcs;
};

void amdgpu_irq_preinstall(struct drm_device *dev);
int amdgpu_irq_postinstall(struct drm_device *dev);
void amdgpu_irq_uninstall(struct drm_device *dev);
irqreturn_t amdgpu_irq_handler(int irq, void *arg);

int amdgpu_irq_init(struct amdgpu_device *adev);
void amdgpu_irq_fini(struct amdgpu_device *adev);
int amdgpu_irq_add_id(struct amdgpu_device *adev, unsigned src_id,
		      struct amdgpu_irq_src *source);
void amdgpu_irq_dispatch(struct amdgpu_device *adev,
			 struct amdgpu_iv_entry *entry);
int amdgpu_irq_update(struct amdgpu_device *adev, struct amdgpu_irq_src *src,
		      unsigned type);
int amdgpu_irq_get(struct amdgpu_device *adev, struct amdgpu_irq_src *src,
		   unsigned type);
bool amdgpu_irq_get_delayed(struct amdgpu_device *adev,
			    struct amdgpu_irq_src *src,
			    unsigned type);
int amdgpu_irq_put(struct amdgpu_device *adev, struct amdgpu_irq_src *src,
		   unsigned type);
bool amdgpu_irq_enabled(struct amdgpu_device *adev, struct amdgpu_irq_src *src,
			unsigned type);

#endif
