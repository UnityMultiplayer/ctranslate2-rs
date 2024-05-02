// convert.h
//
// Copyright (c) 2023-2024 Junpei Kawamoto
//
// This software is released under the MIT License.
//
// http://opensource.org/licenses/mit-license.php

#pragma once

#include "ct2rs/src/types.rs.h"
#include "rust/cxx.h"
#include <string>
#include <vector>

inline std::string from_rust(const rust::Str& v) {
    return static_cast<std::string>(v);
}

inline std::vector<std::string> from_rust(const rust::Vec<rust::Str>& v) {
    std::vector<std::string> res;
    for (const auto& item : v) {
        res.push_back(from_rust(item));
    }
    return res;
}

template <typename VecStr>
inline std::vector<std::vector<std::string>>
from_rust(const rust::Vec<VecStr>& v) {
    std::vector<std::vector<std::string>> res;
    for (const auto& item : v) {
        res.push_back(from_rust(item.v));
    }
    return res;
}

inline std::vector<int> from_rust(const rust::Vec<int>& v) {
    std::vector<int> res;
    for (const auto& item : v) {
        res.push_back(item);
    }
    return res;
}

inline rust::String to_rust(const std::string& v) { return rust::String(v); }

inline rust::Vec<rust::String> to_rust(const std::vector<std::string>& v) {
    rust::Vec<rust::String> res;
    for (const auto& item : v) {
        res.push_back(to_rust(item));
    }
    return res;
}

template <typename T>
inline rust::Vec<T> to_rust(const std::vector<std::vector<std::string>>& v) {
    rust::Vec<T> res;
    for (const auto& item : v) {
        res.push_back(T { to_rust(item) });
    }
    return res;
}

inline rust::Vec<float> to_rust(const std::vector<float>& v) {
    rust::Vec<float> res;
    for (const auto& item : v) {
        res.push_back(item);
    }
    return res;
}

inline rust::Vec<rust::Vec<float>>
to_rust(const std::vector<std::vector<float>>& v) {
    rust::Vec<rust::Vec<float>> res;
    for (const auto& item : v) {
        res.push_back(to_rust(item));
    }
    return res;
}

inline rust::Vec<rust::Vec<rust::Vec<float>>>
to_rust(const std::vector<std::vector<std::vector<float>>>& v) {
    rust::Vec<rust::Vec<rust::Vec<float>>> res;
    for (const auto& item : v) {
        res.push_back(to_rust(item));
    }
    return res;
}

inline rust::Vec<size_t> to_rust(const std::vector<size_t>& v) {
    rust::Vec<size_t> res;
    for (const auto& item : v) {
        res.push_back(item);
    }
    return res;
}

template <typename T>
inline rust::Vec<T> to_rust(const std::vector<std::vector<size_t>>& v) {
    rust::Vec<T> res;
    for (const auto& item : v) {
        res.push_back(T { to_rust(item) });
    }
    return res;
}

inline std::function<bool(ctranslate2::GenerationStepResult)> from_rust(bool has_callback, const rust::Fn<bool(GenerationStepResult)>& callback) {
    if (!has_callback) {
        return nullptr;
    }

    return [=](ctranslate2::GenerationStepResult res) -> bool {
        return callback(GenerationStepResult {
            res.step,
            res.batch_id,
            res.token_id,
            res.hypothesis_id,
            rust::Str(res.token),
            res.log_prob.has_value(),
            res.log_prob.value_or(0),
            res.is_last,
        });
    };
}
