# Contributing Guide

- Contributing to The Ovum Language is fairly easy. This document shows you how to get started

> Note that this repository contains ONLY the toolset integration. That means that the actual tool implementations are in different repositories. For example, the compiler is in the [OvumCompiler](https://github.com/Ovum-Programming-Language/OvumCompiler) repository.
> DO NOT submit PRs to this repository, only to the repositories of the actual tools, unless you are sure that the change is applicable to the toolset integration itself (e.g. introduction of a new tool).

## General
- The [Codebase Structure](./CODEBASE_STRUCTURE.md) has detailed information about how the various files in this project are structured
- Please ensure that any changes you make are in accordance with the [Coding Guidelines](./CODING_GUIDELINES.md) of this repo

## Submitting changes

- Fork the repo (if you're not the maintainer)
  - <https://github.com/Ovum-Programming-Language/OvumLanguage/fork>
- Check out a new branch based and name it to what you intend to do:
  - Example:
    ````
    $ git checkout -b BRANCH_NAME
    ````
    If you get an error, you may need to fetch fooBar first by using
    ````
    $ git remote update && git fetch
    ````
  - Use one branch per fix / feature
- Commit your changes
  - Please provide a git message that explains what you've done
  - Please make sure your commits follow the [Semantic Commit Messages](https://seesparkbox.com/foundry/semantic_commit_messages)
  - Commit to the forked repository
  - Example:
    ````
    $ git commit -am 'Add some fooBar'
    ````
- Push to the branch
  - Example:
    ````
    $ git push origin BRANCH_NAME
    ````
- Make a pull request
  - Make sure you send the PR to the <code>fooBar</code> branch
  - CI is watching you!

If you follow these instructions, your PR will land pretty safely in the main repo!
