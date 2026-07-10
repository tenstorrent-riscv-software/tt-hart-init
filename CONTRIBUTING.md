# Contributing to tt-hart-init

Thank you for your interest in contributing to tt-hart-init! This document
describes how to report issues and submit changes.

## Ways to Contribute

- **Bug Reports**: Report bugs via GitHub Issues
- **Bug Fixes**: Submit fixes via Pull Requests
- **New Features**: Propose and implement new functionality
- **Review**: Review and comment on issues and pull requests
- **Documentation**: Improve the README and code comments

## Reporting Bugs

Please report bugs via
[GitHub Issues](https://github.com/tenstorrent-riscv-software/tt-hart-init/issues)
with:

- A clear, descriptive title
- Steps to reproduce
- Expected and actual behavior
- Your environment (target platform, toolchain and version)
- Any relevant logs or error output

## Suggesting Enhancements

Feature requests are welcome. Please open a GitHub Issue describing the proposed
change, the use case, and any implementation considerations you are aware of.

## Pull Request Process

1. **Fork the repository** and create a branch from `master`.
2. **Make your changes** following the coding standards below.
3. **Build and test** your changes (see [Testing](#testing)).
4. **Write clear commit messages** and sign off your commits (see below).
5. **Open a pull request** against the `master` branch, referencing any related
   issues.

Guidelines:

- Keep each pull request focused on a single feature or fix.
- Explain what changed and why in the description.
- Pull requests are reviewed on a weekly cadence; please be responsive to
  review feedback.

## Coding Standards

- Match the existing code style and conventions in the files you touch.
- Keep functions focused; add comments for non-obvious logic.
- Avoid magic numbers; prefer named constants or macros.
- For assembly, keep register usage and control flow clearly commented, as this
  code runs before a C runtime is available.

## Commit Messages

Write clear, descriptive commit messages:

```
Short summary (50 chars or less)

More detailed explanation if needed, wrapped at 72 characters. Explain the
problem the commit solves and how.
```

## Testing

Before submitting a pull request:

1. **Build** all targets for the platforms your change affects
   (`make all`, with a RISC-V toolchain via `CROSS_COMPILE`).
2. **Test** the boot flow in simulation or on hardware where possible.
3. **Verify** that existing functionality still works.
4. **Describe** the testing you performed in your pull request.

## Licensing

By contributing to tt-hart-init, you agree that your contributions will be
licensed under the Apache License 2.0, the same license as the project. You
retain copyright to your contributions.

### Developer Certificate of Origin

Contributions are accepted under the
[Developer Certificate of Origin](https://developercertificate.org/). Sign off
each commit with `git commit -s` to add a `Signed-off-by` line certifying that:

- You created the contribution and have the right to submit it under the
  Apache 2.0 License, or
- The contribution is based on previous work covered by a compatible open
  source license, or
- The contribution was provided to you by someone who certifies (a) or (b) and
  you are submitting it unmodified.

### AI Attribution

Contributions developed using AI tools should include an `Assisted-by` tag in
the commit message:

    Assisted-by: AGENT_NAME:MODEL_VERSION

where `AGENT_NAME` is the AI tool or framework and `MODEL_VERSION` is the model
version, when it can be identified.

## Code of Conduct

This project adheres to the Contributor Covenant Code of Conduct. By
participating, you are expected to uphold it. Please report unacceptable
behavior to ospo@tenstorrent.com. See [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)
for details.

## Questions?

If you have questions about contributing, open a GitHub Issue.
