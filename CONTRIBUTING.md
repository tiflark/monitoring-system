# Contributing to Monitoring System

## Development Workflow

1. Fork the repository
2. Clone your fork: `git clone git@github.com:your-username/monitoring-system.git`
3. Create a feature branch: `git checkout -b feature/your-feature`
4. Make your changes
5. Run tests: `make test`
6. Commit changes: `git commit -m "feat: your feature description"`
7. Push to your fork: `git push origin feature/your-feature`
8. Create a Pull Request

## Code Style

### C++ Code
- Follow Google C++ Style Guide
- Use meaningful variable names
- Add comments for complex logic
- Write unit tests for new functionality

### Python Code
- Follow PEP 8
- Use type hints
- Add docstrings for functions

## Commit Messages
Use conventional commit messages:
- `feat:` for new features
- `fix:` for bug fixes
- `docs:` for documentation
- `chore:` for maintenance

## Testing
All new code must include tests:
- Unit tests for C++ components
- Integration tests for full system
