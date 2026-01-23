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
        "overclock_v1", "node_clustering", "logic_gate_v2", "quantum_state_fix", "dma_acceleration",
        "packet_burst_v1", "thermal_limiter", "gpu_compute_v1", "isa_refactoring", "microcode_unlock",
        "cache_coherency_v1", "signal_cleaner", "branch_pred_v1", "multi_die_link", "fsb_unlocker",
        "die_shrink_v1", "dark_silicon_fix", "euv_optimization", "clock_domain_v1", "tensor_pipeline",
        "atomic_ops_v1", "sub_zero_cooling", "photonic_bridge", "vacuum_state_fix", "singularity_boot",
        // 1.0.7 hotfixes
	    "neuromorphic_accel", "cryogenic_cache", "spintronics_ctrl", "topological_opt", "quantum_entangle",
	    "nano_wire_sync", "bio_comp_interface", "molecular_logic", "photonic_mem", "quantum_tunneling",
        "gravity_wave_sync", "dark_matter_bus", "tachyon_shredder", "anti_matter_core", "boson_link_v1",
        "higgs_field_tweak", "superstring_opt", "event_horizon_fix", "flux_capacitor_v2", "warp_drive_init",
        "plasma_confinement", "fusion_reactor_v1", "stellar_engine_fix", "dyson_sphere_ctrl", "void_state_v1",
        "entropy_reversal", "causality_fix_v1", "timeline_anchor", "parallel_thread_v1", "multiverse_sync",
        "ethernet_over_void", "quantum_bit_flip_v2", "logic_gate_v3", "parity_fix_v2", "stack_flush_v2",
        "heap_trim_v2", "cache_clear_v2", "core_refactor_v2", "bit_stable_fix_v2", "cycle_reduce_v2",
        "hypervisor_v2", "overclock_v2", "packet_burst_v2", "gpu_compute_v2", "cache_coherency_v2",
        "branch_pred_v2", "die_shrink_v2", "clock_domain_v2", "atomic_ops_v2", "vacuum_state_v1",
        "bit_flip_v2", "gate_bypass_v2", "sub_atomic_tweak", "isotope_stability", "lepton_shredder",
        "quark_alignment", "gluon_binding", "muon_catalysis", "neutrino_mask", "photon_entangle_v2",
        "cybernetic_link", "neural_mesh_v1", "synaptic_relay", "cortex_bypass", "axonic_transfer",
        "biometric_hash", "dna_sequencer_fix", "protein_fold_opt", "genomic_buffer", "cellular_automata",
        "nanite_swarm_v1", "micro_bot_sync", "pico_tech_align", "femto_clock_v1", "atto_second_fix",
        "lattice_vibration", "phonon_cooling", "superconductor_v1", "meissner_effect", "flux_pinning",
        "cryo_stat_v1", "liquid_helium_fix", "nitrogen_flush", "thermal_bleed_fix", "heat_sink_v2",
        "radiator_bypass", "convection_ctrl", "peltier_junction", "exothermic_lock", "absolute_zero_v1",
        "silicon_carbide", "gallium_nitride", "graphene_layer", "carbon_nanotube", "fullerene_mesh",
        "diamond_substrate", "boron_nitride", "molybdenum_fix", "chalcogenide_v1", "perovskite_opt",
        "magnetic_monopole", "ferrofluid_ctrl", "spintronic_bus", "hall_effect_fix", "lorentz_force",
        "tesla_coil_v1", "faraday_cage", "eddy_current_fix", "hysteresis_loop", "gauss_meter_v1",
        "logic_array_v1", "fpga_bitstream", "asic_hardened", "risc_v_ext", "arm_neon_opt",
        "cuda_core_v1", "tensor_unit_v2", "rtx_path_trace", "raster_fix_v1", "vertex_buffer",
        "pixel_pipeline", "shader_clock", "texture_map_v1", "frame_buffer_fix", "v_sync_bypass",
        "hertz_limit_v1", "refresh_rate_fix", "display_port_v2", "hdmi_handshake", "e_edid_spoof",
        "kernel_panic_fix", "shell_escape_v1", "root_access_v2", "sudo_elevation", "privilege_sep",
        "sandbox_escape", "buffer_overflow_v2", "stack_smash_fix", "heap_spray_ctrl", "rop_chain_fix",
        "canary_bypass_v1", "aslr_entropy", "dep_override", "encryption_v1", "cipher_suite_fix",
        "rsa_key_gen", "aes_acceleration", "elliptic_curve", "hash_collision", "salt_pepper_fix",
        "packet_sniffer", "wireshark_opt", "protocol_v1", "tcp_handshake", "udp_flood_fix",
        "icmp_echo_tweak", "dhcp_lease_fix", "dns_poison_fix", "proxy_tunnel_v1", "vpn_handshake",
        "firewall_bypass", "nat_traversal", "port_forward_v1", "socket_bind_fix", "stream_buffer",
        "api_hook_v1", "dll_injection", "so_library_fix", "binary_patch_v1", "hex_editor_opt",
        "disassembler_v1", "debugger_v2", "breakpoint_fix", "trace_route_v1", "log_rotate_v1",
        "garbage_collect", "arc_reference", "smart_ptr_v1", "vector_resize", "hash_map_opt",
        "linked_list_fix", "binary_tree_v1", "graph_traversal", "sorting_algo", "search_optim",
        "thread_pool_v1", "mutex_lock_v1", "semaphore_fix", "deadlock_avoid", "race_condition",
        "context_switch", "interrupt_v1", "irq_balance", "dma_transfer_v2", "pio_mode_fix",
        "bios_flash_v1", "uefi_secure_boot", "nvram_clear", "cmos_reset_v1", "post_code_fix",
        "mbr_partition", "gpt_table_fix", "filesystem_v1", "ntfs_journal", "ext4_resize",
        "zfs_snapshot", "raid_array_v1", "disk_sector_fix", "ssd_trim_v1", "wear_leveling",
        "over_provision", "bad_block_map", "ecc_correction", "parity_check_v3", "checksum_fix",
        "compression_v1", "archive_unpack", "defrag_sweep", "indexing_v1", "metadata_fix",
        "ui_render_v1", "gui_widget_fix", "font_hinting", "anti_aliasing", "color_space_v1",
        "gamma_correct", "hdr_tonemap", "ray_reconstruct", "dlss_patch_v1", "fsr_upscale",
        "audio_buffer_v1", "sample_rate_fix", "latency_lag_fix", "codec_pack_v1", "midi_sync_fix",
        "bit_flip_v3", "gate_bypass_v3", "hypervisor_v3", "packet_burst_v3", "clock_domain_v3",
        "quantum_tunnel_v2", "bio_interface_v2", "molecular_v2", "neuromorphic_v2", "spintronic_v2",
        "photonic_mem_v2", "nano_wire_sync_v2", "cryo_cache_v2", "topological_v2", "singularity_v2"
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