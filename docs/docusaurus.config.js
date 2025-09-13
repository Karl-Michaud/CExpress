// @ts-nocheck
// Note: type annotations allow type checking and IDEs autocompletion
const config = {
  title: 'CExpress',
  tagline: 'A lightweight HTTP server framework for C — bringing the simplicity of Express.js to the power and performance of C.',
  favicon: 'img/favicon.ico',

  // Set the production url of your site here
  url: 'https://Karl-Michaud.github.io',
  // Set the /<baseUrl>/ pathname under which your site is served
  baseUrl: '/CExpress/',

  // GitHub pages deployment config
  organizationName: 'Karl-Michaud',
  projectName: 'CExpress',
  deploymentBranch: 'gh-pages',
  trailingSlash: false,

  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',

  // Even if you don't use internalization, you can use this field to set useful
  // metadata like html lang. For example, if your site is Chinese, you may want
  // to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          sidebarPath: require.resolve('./sidebars.js'),
          editUrl: 'https://github.com/Karl-Michaud/CExpress/tree/documentation/docs/',
        },
      }),
    ],
  ],

  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      navbar: {
        title: 'CExpress',
        // logo: {
        //   alt: 'CExpress Logo',
        //   src: 'img/logo.svg',
        // },
        items: [
          {
            type: 'docSidebar',
            sidebarId: 'tutorialSidebar',
            position: 'left',
            label: 'Docs',
          },
          {
            href: 'https://github.com/Karl-Michaud/CExpress',
            label: 'GitHub',
            position: 'right',
          },
        ],
      },
      footer: {
        style: 'dark',
        links: [
          {
            title: 'Documentation',
            items: [
              {
                label: 'Introduction',
                to: '/docs/intro',
              },
              {
                label: 'Installation',
                to: '/docs/installation',
              },
              {
                label: 'API Reference',
                to: '/docs/api-reference',
              },
              {
                label: 'Usage Examples',
                to: '/docs/usage-examples',
              },
            ],
          },
          {
            title: 'Community',
            items: [
              {
                label: 'GitHub',
                href: 'https://github.com/Karl-Michaud/CExpress',
              },
              {
                label: 'Issues',
                href: 'https://github.com/Karl-Michaud/CExpress/issues',
              },
              {
                label: 'Discussions',
                href: 'https://github.com/Karl-Michaud/CExpress/discussions',
              },
            ],
          },
        ],
        copyright: `${new Date().getFullYear()} Karl-Alexandre Michaud.`,
      },
      prism: {
        additionalLanguages: ['c', 'bash', 'json'],
      },
    }),
};

module.exports = config;
