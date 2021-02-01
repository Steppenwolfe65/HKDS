#include "queue.h"

void qsc_queue_destroy(qsc_queue_state* ctx)
{
	assert(ctx != NULL);

	size_t i;

	if (ctx != NULL)
	{
		for (i = 0; i < ctx->depth; ++i)
		{
			if (ctx->queue[i] != NULL)
			{
				qsc_memutils_clear(ctx->queue[i], ctx->width);
				qsc_memutils_aligned_free(ctx->queue[i]);
			}
		}
	}

	qsc_memutils_aligned_free(ctx->queue);
	qsc_memutils_clear((uint8_t*)ctx->tags, sizeof(ctx->tags));

	ctx->count = 0;
	ctx->depth = 0;
	ctx->position = 0;
	ctx->width = 0;
}

void qsc_queue_flush(qsc_queue_state* ctx, uint8_t* output)
{
	assert(ctx != NULL);
	assert(output != NULL);

	size_t i;

	if (ctx->queue != NULL)
	{
		for (i = 0; i < ctx->position; ++i)
		{
			if (ctx->queue[i] != NULL)
			{
				qsc_memutils_copy(((uint8_t*)output + (i * ctx->width)), ctx->queue[i], ctx->width);
				qsc_memutils_clear(ctx->queue[i], ctx->width);
			}
		}

		ctx->count = 0;
		ctx->position = 0;
		qsc_memutils_clear((uint8_t*)ctx->tags, sizeof(ctx->tags));
	}
}

void qsc_queue_initialize(qsc_queue_state* ctx, size_t depth, size_t width)
{
	assert(ctx != NULL);
	assert(depth != 0 && width != 0);

	size_t i;

	ctx->queue = (uint8_t**)qsc_memutils_aligned_alloc(QSC_QUEUE_ALIGNMENT, depth * sizeof(uint8_t*));

	if (ctx->queue != NULL)
	{
		for (i = 0; i < depth; ++i)
		{
			ctx->queue[i] = qsc_memutils_aligned_alloc(QSC_QUEUE_ALIGNMENT, width);

			if (ctx->queue[i] != NULL)
			{
				qsc_memutils_clear(ctx->queue[i], width);
			}
		}

		ctx->count = 0;
		ctx->depth = depth;
		ctx->position = 0;
		qsc_memutils_clear((uint8_t*)ctx->tags, QSC_QUEUE_MAX_DEPTH);
		ctx->width = width;
	}
}

size_t qsc_queue_items(qsc_queue_state* ctx)
{
	assert(ctx != NULL);

	return ctx->count;
}

bool qsc_queue_isfull(qsc_queue_state* ctx)
{
	assert(ctx != NULL);

	return (bool)(ctx->count == ctx->depth);
}

bool qsc_queue_isempty(qsc_queue_state* ctx)
{
	assert(ctx != NULL);

	return (bool)(ctx->count == 0);
}

uint64_t qsc_queue_pop(qsc_queue_state* ctx, uint8_t* output, size_t outlen)
{
	assert(ctx != NULL);
	assert(output != NULL);
	assert(outlen != 0);

	uint64_t tag;
	size_t i;

	tag = 0;

	if (!qsc_queue_isempty(ctx) && outlen <= ctx->width)
	{
		qsc_memutils_copy(output, (uint8_t*)ctx->queue[0], outlen);
		qsc_memutils_clear((uint8_t*)ctx->queue[0], ctx->width);
		tag = ctx->tags[ctx->position - 1];

		if (ctx->count > 1)
		{
			for (i = 1; i < ctx->count; ++i)
			{
				qsc_memutils_copy((uint8_t*)ctx->queue[i - 1], (uint8_t*)ctx->queue[i], ctx->width);
				ctx->tags[i - 1] = ctx->tags[i];
			}
		}

		qsc_memutils_clear((uint8_t*)ctx->queue[ctx->position - 1], ctx->width);
		ctx->tags[ctx->position - 1] = 0;
		--ctx->count;
		--ctx->position;
	}

	return tag;
}

void qsc_queue_push(qsc_queue_state* ctx, const uint8_t* input, size_t inlen, uint64_t tag)
{
	assert(ctx != NULL);
	assert(input != NULL);
	assert(inlen != 0);

	if (!qsc_queue_isfull(ctx) && inlen <= ctx->width)
	{
		qsc_memutils_copy((uint8_t*)ctx->queue[ctx->position], input, inlen);
		ctx->tags[ctx->position] = tag;
		++ctx->position;
		++ctx->count;
	}
}

bool qsc_queue_self_test()
{
	uint8_t exp[64][16] = { 0 };
	uint8_t otp1[64 * 16] = { 0 };
	uint8_t otp2[64][16] = { 0 };
	uint32_t tag[64] = { 0 };
	qsc_queue_state ctx;
	size_t i;
	size_t j;
	bool res;

	res = true;
	qsc_queue_initialize(&ctx, 64, 16);


	for (i = 0; i < 64; ++i)
	{
		for (j = 0; j < 16; ++j)
		{
			exp[i][j] = (uint8_t)(i + j);
		}
	}

	for (i = 0; i < 64; ++i)
	{
		qsc_queue_push(&ctx, exp[i], 16, i);
	}

	if (qsc_queue_isfull(&ctx) == false)
	{
		res = false;
	}

	for (i = 0; i < 64; ++i)
	{
		qsc_queue_pop(&ctx, otp2[i], 16);
	}

	if (qsc_queue_isempty(&ctx) == false)
	{
		res = false;
	}

	if (qsc_queue_items(&ctx) != 0)
	{
		res = false;
	}

	for (i = 0; i < 64; ++i)
	{
		if (qsc_intutils_are_equal8(exp[i], otp2[i], 16) == false)
		{
			res = false;
			break;
		}
	}

	for (i = 0; i < 64; ++i)
	{
		qsc_queue_push(&ctx, exp[i], 16, i);
	}

	if (qsc_queue_items(&ctx) != 64)
	{
		res = false;
	}

	qsc_queue_flush(&ctx, otp1);

	for (i = 0; i < 64; ++i)
	{
		if (qsc_intutils_are_equal8(exp[i], ((uint8_t*)otp1 + i * 16), 16) == false)
		{
			res = false;
			break;
		}
	}

	qsc_queue_destroy(&ctx);

	return res;
}
