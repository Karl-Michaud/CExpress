# CExpress Documentation

This directory contains the Docusaurus documentation site for CExpress.

## Development

### Prerequisites

- Node.js 18 or later
- npm or yarn

### Setup

```bash
# Install dependencies
npm install

# Start development server
npm start

# Build for production
npm run build

# Serve production build locally
npm run serve
```

## Structure

```
docs/
├── docs/                    # Documentation pages
│   ├── getting-started/     # Getting started guides
│   ├── core-concepts/       # Core concepts
│   ├── api/                 # API reference
│   ├── examples/            # Examples
│   └── advanced/            # Advanced topics
├── src/                     # React components
│   ├── components/          # Reusable components
│   ├── pages/               # Custom pages
│   └── css/                 # Custom styles
├── static/                  # Static assets
│   └── img/                 # Images and icons
├── docusaurus.config.js     # Docusaurus configuration
├── sidebars.js              # Sidebar configuration
└── package.json             # Dependencies
```

## Deployment

The documentation is automatically deployed to GitHub Pages when changes are pushed to the main branch.

### Manual Deployment

```bash
# Build the site
npm run build

# Deploy to GitHub Pages
npm run deploy
```

## Contributing

1. Make changes to documentation files
2. Test locally with `npm start`
3. Commit and push changes
4. Documentation will be automatically deployed

## Configuration

The site is configured in `docusaurus.config.js`:

- **URL**: https://karlosins.github.io/CExpress/
- **Base URL**: /CExpress/
- **Organization**: karlosins
- **Project**: CExpress

## Features

- **Responsive design** - Works on all devices
- **Dark mode** - Automatic theme switching
- **Search** - Algolia search integration (configured)
- **Versioning** - Support for multiple versions
- **Blog** - Built-in blog functionality
- **Custom pages** - Custom React components

## Customization

### Adding New Pages

1. Create a new `.md` file in the appropriate directory
2. Add the page to `sidebars.js`
3. The page will appear in the navigation

### Custom Components

Add React components in `src/components/` and import them in pages.

### Styling

Custom CSS can be added to `src/css/custom.css`.

## Troubleshooting

### Common Issues

**Build fails:**
```bash
# Clear cache and reinstall
rm -rf node_modules package-lock.json
npm install
```

**Search not working:**
- Check Algolia configuration in `docusaurus.config.js`
- Ensure search index is built

**Images not loading:**
- Place images in `static/img/`
- Use relative paths: `/img/filename.png`

## Support

For documentation issues:

- Check the [Docusaurus documentation](https://docusaurus.io/docs)
- Open an issue in the [CExpress repository](https://github.com/karlosins/CExpress/issues)
- Join the [Discussions](https://github.com/karlosins/CExpress/discussions)
