#pragma once

#include "../Misc/GIncludes.hpp"

struct Hotfix {
    std::string name;
    long double bits;
    float bitMult;
    int written = 0;
    sf::RectangleShape rect;

    Hotfix() : bits(0), bitMult(0), written(0) {}

    Hotfix(std::string n, long double c, float m)
        : name(n), bits(c), bitMult(m), written(0) {
        rect.setSize({ 120.f, 80.f });
        rect.setOutlineColor(sf::Color(243, 238, 225));
        rect.setOutlineThickness(2.f);
    }
};

inline std::vector<Hotfix> hotfixList;
inline const float hfWidth = 120.0f;
inline const float hfHeight = 80.0f;
inline const float hfSpacing = 15.0f;

inline void initHotfixes() {
    std::vector<std::string> names = {
        // 1.0.0 hotfixes
        "init_patch", "boot_optim", "cache_clear", "core_refactor", "thread_sync",
        "heap_trim", "stack_flush", "bit_stable_fix", "io_throttle", "cycle_reduce",
        "bit_flip_v1", "overflow_ctrl", "registry_tweak", "nullptr_fix", "parity_fix",
        "mem_compress", "cycle_skip", "runtime_opt", "clock_spd_fix", "bus_overdrive",
        "ptr_deref_opt", "seg_fault_supp", "bin_compact", "gate_bypass_v1", "fetch_accel",
        "patch_x86_ext", "mem_map_align", "syscall_bypass", "data_leak_fix", "inst_set_ext",
        // 1.0.2 hotfixes
        "hypervisor_v1", "kernel_bypass", "ring_zero_access", "vram_injection", "latency_shred",
        "overclock_v1," "node_clustering", "logic_gate_v2", "quantum_state_fix", "dma_acceleration",
        "packet_burst_v1", "thermal_limiter", "gpu_compute_v1", "isa_refactoring", "microcode_unlock",
        "cache_coherency_v1", "signal_cleaner", "branch_pred_v1", "multi_die_link", "fsb_unlocker",
        "die_shrink_v1", "dark_silicon_fix", "euv_optimization", "clock_domain_v1", "tensor_pipeline",
        "atomic_ops_v1", "sub_zero_cooling", "photonic_bridge", "vacuum_state_fix", "singularity_boot",
        // 1.0.7 hotfixes
    };

    long double currentBits = 100.0L;
    float mult = 0.05f;

    for (const auto& n : names) {
        hotfixList.push_back({ n, currentBits, mult });
        currentBits *= hotfixGrowth;
    }
}

inline void to_json(json& j, const Hotfix& h) {
    j = json{ {"n", h.name}, {"w", h.written} };
}

inline void from_json(const json& j, Hotfix& h) {
    j.at("n").get_to(h.name);
    j.at("w").get_to(h.written);
}