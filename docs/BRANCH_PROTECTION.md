# Default-branch ruleset: prepared, not active

On 2026-10-26, GitHub returned HTTP 403 for both the repository rulesets and
classic branch-protection endpoints, with the message:

> Upgrade to GitHub Pro or make this repository public to enable this feature.

The repository remains private. No subscription or visibility change has been
made. This file and the JSON configuration do not enable GitHub enforcement.
The ruleset has not been accepted or validated by the server.

## Prepared baseline

Import `.github/rulesets/main-protection.json` once this private repository is
eligible for rulesets. It is intended to activate the following behavior on
the default branch, currently `main`:

- Changes must arrive through pull requests. Topic branches remain available
  for contributors with write access.
- One approving review is required, and review discussions must be resolved.
- New reviewable commits dismiss existing approvals.
- Branch deletion and non-fast-forward pushes are blocked.
- Repository administrators can bypass review requirements only through a PR;
  the template does not grant them direct-push bypass. This keeps solo-owner
  maintenance possible while preserving a PR record.

The administrator role is GitHub's built-in RepositoryRole ID 5. Ordinary write
or maintain roles and GitHub Apps are not on the bypass list. No collaborator
access has been granted by this change.

Required CI checks are deliberately absent because the repository currently
has no GitHub Actions workflows or check contexts. Once real CI is installed
and verified, add its exact check names to a separate ruleset update. CODEOWNERS
reviews, commit signing and deployment requirements are not part of this basic
profile.

## Activation

After the repository becomes eligible, open **Settings → Rules → Rulesets →
New ruleset → Import a ruleset**, select the JSON, and review the target,
admin PR-only bypass, approvals and enforcement status before saving.

Before API creation, list the repository's existing rulesets and reuse/update
a matching one rather than creating a duplicate. After saving, read the
server-side configuration back and verify its active enforcement and target.
Committing or merging the JSON alone does not protect the branch.

Official references:

- [Ruleset availability and creation](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/creating-rulesets-for-a-repository)
- [Available branch rules](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/available-rules-for-rulesets)
- [Rulesets REST API](https://docs.github.com/en/rest/repos/rules#create-a-repository-ruleset)
