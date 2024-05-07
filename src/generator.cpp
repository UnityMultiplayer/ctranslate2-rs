// generator.cpp
//
// Copyright (c) 2023-2024 Junpei Kawamoto
//
// This software is released under the MIT License.
//
// http://opensource.org/licenses/mit-license.php

#include "ct2rs/include/generator.h"

#include "ct2rs/include/convert.h"
#include "ct2rs/src/generator.rs.h"

using rust::Str;
using rust::Vec;
using std::string;
using std::vector;

inline std::function<bool(ctranslate2::GenerationStepResult)> convert_callback(
    bool has_callback,
    GenerationCallbackBox& callback
) {
    if (!has_callback) {
        return nullptr;
    }

    return [&](ctranslate2::GenerationStepResult res) -> bool {
        return execute_generation_callback(
            callback,
            GenerationStepResult {
                res.step,
                res.batch_id,
                res.token_id,
                res.hypothesis_id,
                rust::String(res.token),
                res.log_prob.has_value(),
                res.log_prob.value_or(0),
                res.is_last,
            }
        );
    };
}

Vec<GenerationResult>
Generator::generate_batch(
    const Vec<VecStr>& start_tokens,
    const GenerationOptions& options,
    bool has_callback,
    GenerationCallbackBox& callback
) const {
    auto futures = this->impl->generate_batch_async(
        from_rust(start_tokens),
        ctranslate2::GenerationOptions {
            options.beam_size,
            options.patience,
            options.length_penalty,
            options.repetition_penalty,
            options.no_repeat_ngram_size,
            options.disable_unk,
            from_rust(options.suppress_sequences),
            {},
            options.return_end_token,
            options.max_length,
            options.min_length,
            options.sampling_topk,
            options.sampling_topp,
            options.sampling_temperature,
            options.num_hypotheses,
            options.return_scores,
            options.return_alternatives,
            options.min_alternative_expansion_prob,
            from_rust(options.static_prompt),
            options.cache_static_prompt,
            options.include_prompt_in_result,
            convert_callback(has_callback, callback),
        },
        options.max_batch_size,
        options.batch_type
    );

    Vec<GenerationResult> res;
    for (auto& future : futures) {
        const auto& r = future.get();
        res.push_back(GenerationResult {
            to_rust<VecString>(r.sequences),
            to_rust<VecUSize>(r.sequences_ids),
            to_rust(r.scores),
        });
    }

    return res;
}
