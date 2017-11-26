import helpers from './helpers';

export default
{
    get(url, data, complete, config, error)
    {
        return this.request('get', url, data, complete, config, error);
    },

    post(url, data, complete, config, error)
    {
        return this.request('post', url, data, complete, config, error);
    },

    put(url, data, complete, config, error)
    {
        return this.request('put', url, data, complete, config, error);
    },

    delete(url, data, complete, config, error)
    {
        return this.request('delete', url, data, complete, config, error);
    },

    request(method, url, data, complete, config, error)
    {
        if (config !== 'object')
        {
            config = {};
        }

        let query = '';
        if (method.toLowerCase() === 'get')
        {
            $.each(data, function(key, value)
            {
                query += encodeURIComponent(key) + '=' + encodeURIComponent(value) + '&';
            });

            if (query !== '')
            {
                query = (url.includes('?') ? '&' : '?') + query.slice(0, -1);
            }

            data = {};
        }

        $.extend(config,
            {
                url: '/api' + url + query,
                method: method,
                data: data,
            });

        return axios.request(config).then(function(response)
        {
            if (complete)
            {
                complete(response.data, response);
            }
        }).catch(function(err)
        {
            if (error)
            {
                if (error(err) === false)
                {
                    return;
                }
            }

            $('.ui.modal').modal('hide');

            if (typeof err.response === 'object' && typeof err.response.data === 'object')
            {
                let data = err.response.data;

                if (typeof data.message === 'string')
                {
                    helpers.alert('error', 'Error', data.message);
                }

                if (typeof data.errors === 'object')
                {
                    $.each(data.errors, (key, msg) =>
                    {
                        $('[name=' + key + ']').eq(0).popup({
                            content: msg[0],
                            position: 'right center',
                            on: 'manual'
                        }).popup('show');
                    });
                }
            }
            else
            {
                helpers.alert('error', 'Error', 'Please check the console for more information.');
                throw err;
            }
        });
    }
}