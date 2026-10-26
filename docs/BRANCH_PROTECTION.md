# Default-branch ruleset: active

The repository became public on 2026-10-26. GitHub accepted and activated
ruleset **25910947**, and the active-rules endpoint confirms that it applies
to `main`. [View the server-side ruleset](https://github.com/RobCZart82/yaman1x/rules/25910947).

Before publication, GitHub returned HTTP 403 for rulesets and classic branch
protection because this private repository was not eligible under the account
plan. No subscription change was made. Public repository availability removed
that limitation.

The JSON file remains the reusable baseline; the actual protection is the
active server-side ruleset, read back and verified after creation.

## Active baseline

The configuration in `.github/rulesets/main-protection.json` now enforces
the following baseline on the default branch, currently `main`:

- Changes must arrive through pull requests. Topic branches remain available
  for contributors with write access.
- No approving review is required for now, so the sole maintainer can merge
  their own PRs. Review discussions must still be resolved.
- New commits dismiss existing approvals if any have been given.
- Branch deletion and non-fast-forward pushes are blocked.
- The administrator bypass mode is PR-only, so administrators still cannot
  push directly to the default branch. This preserves a PR record while
  allowing solo-owner maintenance.

The administrator role is GitHub's built-in RepositoryRole ID 5. Ordinary write
or maintain roles and GitHub Apps are not on the bypass list. No collaborator
access has been granted by this change.

Required CI checks are deliberately absent because the repository currently
has no GitHub Actions workflows or check contexts. Once real CI is installed
and verified, add its exact check names to a separate ruleset update. CODEOWNERS
reviews, commit signing and deployment requirements are not part of this basic
profile.

## Reusing the configuration

For another eligible repository, open **Settings → Rules → Rulesets →
New ruleset → Import a ruleset**, select the JSON, and review the target,
admin PR-only bypass, zero required approvals and enforcement status before
saving.

Before API creation, list the repository's existing rulesets and reuse/update
a matching one rather than creating a duplicate. After saving, read the
server-side configuration back and verify its active enforcement and target.
Committing or merging the JSON alone does not protect the branch.

Official references:

- [Ruleset availability and creation](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/creating-rulesets-for-a-repository)
- [Available branch rules](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/available-rules-for-rulesets)
- [Rulesets REST API](https://docs.github.com/en/rest/repos/rules#create-a-repository-ruleset)
