# mpact-sim Agent Instructions

## Lessons Learned


### Lessons Learned
- **Test Environment Masking via Build Cache Bypass:** Ensure `harness.yaml` test targets include `--nocache_test_results` to prevent bypassed cached executions.
