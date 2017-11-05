import api from './api';
import helpers from './helpers';

export default
{
    submit(form, complete, config)
    {
        let $form = $(form), data,
            redirect = helpers.truth(form.getAttribute('data-redirect') || true);

        if (form.getAttribute('enctype') === 'multipart/form-data')
        {
            data = new FormData;

            $form.find('[name]').each(function(k, obj)
            {
                let t = $(obj);

                if (t.attr('type') !== 'file')
                {
                    data.append(t.attr('name'), t.val());
                }
                else
                {
                    for (let i = 0; i < t[0].files.length; i++)
                    {
                        data.append(t.attr('name'), t[0].files[i]);
                    }
                }
            });
        }
        else
        {
            data = $form.serialize();
        }

        $form.find('[name]').popup('destroy');

        api.request(form.getAttribute('method') || 'get', form.getAttribute('action'), data, (data, response) =>
        {
            if (complete)
            {
                if (complete(data, response) === false)
                {
                    return;
                }
            }

            if (typeof data.warning === 'string')
            {
                helpers.alert('warning', 'Warning', data.warning);
            }
            else
            {
                helpers.alert('success', 'Success', data.message || 'Changes have been saved successfully.', 2000);
            }

            if (redirect && typeof data.redirect === 'string')
            {
                setTimeout(() =>
                {
                    app.$router.push({path: data.redirect});
                });
            }
        }, config);
    }
}