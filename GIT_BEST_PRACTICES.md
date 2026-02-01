# Git & Version Control Best Practices

To demonstrate professional development standards for your college project, follow these guidelines. This will show your professor you understand software engineering workflows.

## 1. Branching Strategy (GitFlow Lite)
Never push directly to `main`. Use branches to keep your work organized and safe.

- **`main`**: The "production-ready" code. Only functional, tested code goes here.
- **`develop`**: The integration branch. Merge your features here first. (Optional for smaller solo projects, but recommended).
- **`feature/your-feature-name`**: Where you do the actual work.

### Workflow Example:
1.  **Start a task**: `git checkout -b feature/setup-http-client`
2.  **Do the work**: Write code, compile, test.
3.  **Save progress**: `git commit -m "feat(network): implement basic http fetch"`
4.  **Finish task**: Switch to main/develop and merge (or use Pull Requests on GitHub).

## 2. Conventional Commits
Use a structured format for your commit messages. This makes the history readable and generates automatic changelogs.

**Format**: `<type>(<scope>): <short description>`

### Common Types:
- **`feat`**: A new feature (e.g., `feat: add orbit calculator`)
- **`fix`**: A bug fix (e.g., `fix: resolve crash on invalid json`)
- **`docs`**: Documentation only (e.g., `docs: update readme with setup instructions`)
- **`style`**: Formatting, missing semi-colons, etc; no code change (e.g., `style: format main.cpp`)
- **`refactor`**: Refactoring production code (e.g., `refactor: move json parsing to separate class`)
- **`test`**: Adding missing tests, refactoring tests (e.g., `test: add unit test for date parser`)
- **`chore`**: Updating build tasks, package manager configs, etc (e.g., `chore: update cmake dependencies`)

### Examples:
- `feat(ui): add asteroid list panel`
- `fix(parser): handle missing api fields`
- `docs(readme): add screenshot of dashboard`

## 3. GitHub Workflow
1.  **Create a Repository** on GitHub (Empty, since we have local files).
2.  **Push** your local repo to GitHub.
3.  **Use Pull Requests (PRs)**: Even if working alone, creating a PR from `feature/xyz` to `main` allows you to review your own code and show a history of code reviews.

## 4. Atomic Commits
Commit one logical change at a time.
- **Bad**: "Fix bugs and add login and update style" (Too big, hard to revert).
- **Good**: Three separate commits for the above.
