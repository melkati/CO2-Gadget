/**
 * Change the theme of the page.
 */
function toggleTheme() {
    const htmlElement = document.documentElement;
    const currentTheme = htmlElement.getAttribute('theme');
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';

    console.debug('Toggle theme function called');
    console.debug('Current theme:', currentTheme);
    console.debug('New theme:', newTheme);

    htmlElement.setAttribute('theme', newTheme);
    console.debug('Theme set to:', newTheme);

    // Save the selected theme to localStorage
    localStorage.setItem('theme', newTheme);
}

/**
 * Adjust the padding-top of the content based on the navbar height.
 */
function adjustContentPadding() {
    const navbar = document.querySelector('.navbar');
    const content = document.querySelector('.content');

    if (navbar && content) {
        const navbarHeight = navbar.offsetHeight;
        content.style.paddingTop = `${navbarHeight}px`;
    }
}

/**
 * Initialize the theme from localStorage.
 */
function initializeTheme() {
    const savedTheme = localStorage.getItem('theme');
    if (savedTheme) {
        document.documentElement.setAttribute('theme', savedTheme);
    }
}

/**
 * Initialize event listeners for the theme switch.
 */
function initializeThemeSwitch() {
    const colorSwitch = document.querySelector("#iconBulb");
    if (colorSwitch) {
        colorSwitch.addEventListener('click', toggleTheme);
    } else {
        console.error('Element with ID "iconBulb" not found.');
    }
}

document.addEventListener("DOMContentLoaded", function () {
    initializeTheme();
    initializeThemeSwitch();
});