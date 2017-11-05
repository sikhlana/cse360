
window._ = require('lodash');

/**
 * We'll load jQuery and the Bootstrap jQuery plugin which provides support
 * for JavaScript based Bootstrap features such as modals and tabs. This
 * code may be modified to fit the specific needs of your application.
 */

try {
    window.$ = window.jQuery = require('jquery');

    require('semantic-ui-css');
} catch (e) {}

/**
 * We'll load the axios HTTP library which allows us to easily issue requests
 * to our Laravel back-end. This library automatically handles sending the
 * CSRF token as a header based on the value of the "XSRF" token cookie.
 */

window.axios = require('axios');

window.axios.defaults.headers.common['X-Requested-With'] = 'XMLHttpRequest';

/**
 * Import all the third-party plug-ins.
 */

window.$.context = function(fn, context, extra)
{
    if (typeof context === 'string')
    {
        let _context = fn;
        fn = fn[context];
        context = _context;
    }

    return function()
    {
        if (typeof extra === 'object')
        {
            for (let i = extra.length - 1; i >= 0; i--)
            {
                Array.prototype.unshift.call(arguments, extra[i]);
            }
        }

        return fn.apply(context, arguments);
    };
};
