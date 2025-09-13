"use strict";
/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
Object.defineProperty(exports, "__esModule", { value: true });
exports.validateOptions = void 0;
const tslib_1 = require("tslib");
const path_1 = tslib_1.__importDefault(require("path"));
const logger_1 = tslib_1.__importDefault(require("@docusaurus/logger"));
const utils_1 = require("@docusaurus/utils");
const blogUtils_1 = require("./blogUtils");
const footnoteIDFixer_1 = tslib_1.__importDefault(require("./remark/footnoteIDFixer"));
const translations_1 = require("./translations");
const feed_1 = require("./feed");
const routes_1 = require("./routes");
async function pluginContentBlog(context, options) {
    const { siteDir, siteConfig, generatedFilesDir, localizationDir, i18n: { currentLocale }, } = context;
    const { onBrokenMarkdownLinks, baseUrl } = siteConfig;
    const contentPaths = {
        contentPath: path_1.default.resolve(siteDir, options.path),
        contentPathLocalized: (0, utils_1.getPluginI18nPath)({
            localizationDir,
            pluginName: 'docusaurus-plugin-content-blog',
            pluginId: options.id,
        }),
    };
    const pluginId = options.id ?? utils_1.DEFAULT_PLUGIN_ID;
    const pluginDataDirRoot = path_1.default.join(generatedFilesDir, 'docusaurus-plugin-content-blog');
    const dataDir = path_1.default.join(pluginDataDirRoot, pluginId);
    // TODO Docusaurus v4 breaking change
    //  module aliasing should be automatic
    //  we should never find local absolute FS paths in the codegen registry
    const aliasedSource = (source) => `~blog/${(0, utils_1.posixPath)(path_1.default.relative(pluginDataDirRoot, source))}`;
    const authorsMapFilePath = await (0, utils_1.getDataFilePath)({
        filePath: options.authorsMapPath,
        contentPaths,
    });
    return {
        name: 'docusaurus-plugin-content-blog',
        getPathsToWatch() {
            const { include } = options;
            const contentMarkdownGlobs = (0, utils_1.getContentPathList)(contentPaths).flatMap((contentPath) => include.map((pattern) => `${contentPath}/${pattern}`));
            return [authorsMapFilePath, ...contentMarkdownGlobs].filter(Boolean);
        },
        getTranslationFiles() {
            return (0, translations_1.getTranslationFiles)(options);
        },
        // Fetches blog contents and returns metadata for the necessary routes.
        async loadContent() {
            const { postsPerPage: postsPerPageOption, routeBasePath, tagsBasePath, blogDescription, blogTitle, blogSidebarTitle, pageBasePath, } = options;
            const baseBlogUrl = (0, utils_1.normalizeUrl)([baseUrl, routeBasePath]);
            const blogTagsListPath = (0, utils_1.normalizeUrl)([baseBlogUrl, tagsBasePath]);
            let blogPosts = await (0, blogUtils_1.generateBlogPosts)(contentPaths, context, options);
            blogPosts = await (0, blogUtils_1.applyProcessBlogPosts)({
                blogPosts,
                processBlogPosts: options.processBlogPosts,
            });
            const listedBlogPosts = blogPosts.filter(blogUtils_1.shouldBeListed);
            if (!blogPosts.length) {
                return {
                    blogSidebarTitle,
                    blogPosts: [],
                    blogListPaginated: [],
                    blogTags: {},
                    blogTagsListPath,
                };
            }
            // Collocate next and prev metadata.
            listedBlogPosts.forEach((blogPost, index) => {
                const prevItem = index > 0 ? listedBlogPosts[index - 1] : null;
                if (prevItem) {
                    blogPost.metadata.prevItem = {
                        title: prevItem.metadata.title,
                        permalink: prevItem.metadata.permalink,
                    };
                }
                const nextItem = index < listedBlogPosts.length - 1
                    ? listedBlogPosts[index + 1]
                    : null;
                if (nextItem) {
                    blogPost.metadata.nextItem = {
                        title: nextItem.metadata.title,
                        permalink: nextItem.metadata.permalink,
                    };
                }
            });
            const blogListPaginated = (0, blogUtils_1.paginateBlogPosts)({
                blogPosts: listedBlogPosts,
                blogTitle,
                blogDescription,
                postsPerPageOption,
                basePageUrl: baseBlogUrl,
                pageBasePath,
            });
            const blogTags = (0, blogUtils_1.getBlogTags)({
                blogPosts,
                postsPerPageOption,
                blogDescription,
                blogTitle,
                pageBasePath,
            });
            return {
                blogSidebarTitle,
                blogPosts,
                blogListPaginated,
                blogTags,
                blogTagsListPath,
            };
        },
        async contentLoaded({ content, actions }) {
            await (0, routes_1.createAllRoutes)({
                baseUrl,
                content,
                actions,
                options,
                aliasedSource,
            });
        },
        translateContent({ content, translationFiles }) {
            return (0, translations_1.translateContent)(content, translationFiles);
        },
        configureWebpack(_config, isServer, utils, content) {
            const { admonitions, rehypePlugins, remarkPlugins, truncateMarker, beforeDefaultRemarkPlugins, beforeDefaultRehypePlugins, } = options;
            const markdownLoaderOptions = {
                siteDir,
                contentPaths,
                truncateMarker,
                sourceToPermalink: (0, blogUtils_1.getSourceToPermalink)(content.blogPosts),
                onBrokenMarkdownLink: (brokenMarkdownLink) => {
                    if (onBrokenMarkdownLinks === 'ignore') {
                        return;
                    }
                    logger_1.default.report(onBrokenMarkdownLinks) `Blog markdown link couldn't be resolved: (url=${brokenMarkdownLink.link}) in path=${brokenMarkdownLink.filePath}`;
                },
            };
            const contentDirs = (0, utils_1.getContentPathList)(contentPaths);
            return {
                resolve: {
                    alias: {
                        '~blog': pluginDataDirRoot,
                    },
                },
                module: {
                    rules: [
                        {
                            test: /\.mdx?$/i,
                            include: contentDirs
                                // Trailing slash is important, see https://github.com/facebook/docusaurus/pull/3970
                                .map(utils_1.addTrailingPathSeparator),
                            use: [
                                {
                                    loader: require.resolve('@docusaurus/mdx-loader'),
                                    options: {
                                        admonitions,
                                        remarkPlugins,
                                        rehypePlugins,
                                        beforeDefaultRemarkPlugins: [
                                            footnoteIDFixer_1.default,
                                            ...beforeDefaultRemarkPlugins,
                                        ],
                                        beforeDefaultRehypePlugins,
                                        staticDirs: siteConfig.staticDirectories.map((dir) => path_1.default.resolve(siteDir, dir)),
                                        siteDir,
                                        isMDXPartial: (0, utils_1.createAbsoluteFilePathMatcher)(options.exclude, contentDirs),
                                        metadataPath: (mdxPath) => {
                                            // Note that metadataPath must be the same/in-sync as
                                            // the path from createData for each MDX.
                                            const aliasedPath = (0, utils_1.aliasedSitePath)(mdxPath, siteDir);
                                            return path_1.default.join(dataDir, `${(0, utils_1.docuHash)(aliasedPath)}.json`);
                                        },
                                        // For blog posts a title in markdown is always removed
                                        // Blog posts title are rendered separately
                                        removeContentTitle: true,
                                        // Assets allow to convert some relative images paths to
                                        // require() calls
                                        createAssets: ({ frontMatter, metadata, }) => ({
                                            image: frontMatter.image,
                                            authorsImageUrls: metadata.authors.map((author) => author.imageURL),
                                        }),
                                        markdownConfig: siteConfig.markdown,
                                    },
                                },
                                {
                                    loader: path_1.default.resolve(__dirname, './markdownLoader.js'),
                                    options: markdownLoaderOptions,
                                },
                            ].filter(Boolean),
                        },
                    ],
                },
            };
        },
        async postBuild({ outDir, content }) {
            if (!options.feedOptions.type) {
                return;
            }
            const { blogPosts } = content;
            if (!blogPosts.length) {
                return;
            }
            await (0, feed_1.createBlogFeedFiles)({
                blogPosts,
                options,
                outDir,
                siteConfig,
                locale: currentLocale,
            });
        },
        injectHtmlTags({ content }) {
            if (!content.blogPosts.length || !options.feedOptions.type) {
                return {};
            }
            const feedTypes = options.feedOptions.type;
            const feedTitle = options.feedOptions.title ?? context.siteConfig.title;
            const feedsConfig = {
                rss: {
                    type: 'application/rss+xml',
                    path: 'rss.xml',
                    title: `${feedTitle} RSS Feed`,
                },
                atom: {
                    type: 'application/atom+xml',
                    path: 'atom.xml',
                    title: `${feedTitle} Atom Feed`,
                },
                json: {
                    type: 'application/json',
                    path: 'feed.json',
                    title: `${feedTitle} JSON Feed`,
                },
            };
            const headTags = [];
            feedTypes.forEach((feedType) => {
                const { type, path: feedConfigPath, title: feedConfigTitle, } = feedsConfig[feedType];
                headTags.push({
                    tagName: 'link',
                    attributes: {
                        rel: 'alternate',
                        type,
                        href: (0, utils_1.normalizeUrl)([
                            baseUrl,
                            options.routeBasePath,
                            feedConfigPath,
                        ]),
                        title: feedConfigTitle,
                    },
                });
            });
            return {
                headTags,
            };
        },
    };
}
exports.default = pluginContentBlog;
var options_1 = require("./options");
Object.defineProperty(exports, "validateOptions", { enumerable: true, get: function () { return options_1.validateOptions; } });
