// @ts-nocheck
const config = {
  title: 'CExpress',
  tagline: 'A lightweight HTTP server framework for C that brings the simplicity of Express.js to the power and performance of C.',
  url: 'https://karl-michaud.github.io',
  baseUrl: '/CExpress/',

  organizationName: 'Karl-Michaud', // GitHub username
  projectName: 'CExpress',          // Repo name

  onBrokenLinks: 'warn',
  onBrokenMarkdownLinks: 'warn',

  presets: [
    [
      'classic',
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          path: 'docs', // This matches your folder: /docs/docs/
          routeBasePath: '/', // Homepage is index.md at /
          sidebarPath: require.resolve('./sidebars.js'),
        },
        blog: false,
      }),
    ],
  ],
};
  
module.exports = config;
